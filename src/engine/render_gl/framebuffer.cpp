/*
 * framebuffer.cpp
 *
 *  Created on: Jul 9, 2022
 *      Author: alex9932
 */

#include <engine/render_gl/framebuffer.h>
#include <engine/render/window.h>
#include <engine/core/math/math.h>

namespace Engine {
	namespace Render {

		GLFramebuffer::GLFramebuffer(const FramebufferInfo& info) : Framebuffer(info) {
			this->width = info.width;
			this->height = info.height;

			glGenFramebuffers(1, &this->fb);

			if(info.attachment_count == 0 || info.attachment_count > RG_GL_FBATTACHMENTS) {
				rgLogError(RG_LOG_RENDER, "INVALID FRAMEBUFFER ATTACHMENT COUNT!");
			}

			this->attachment_count = info.attachment_count;
			glGenTextures(this->attachment_count, this->attachments);

			glBindFramebuffer(GL_FRAMEBUFFER, this->fb);

			GLenum color_buffers[this->attachment_count];

			for (Uint32 i = 0; i < this->attachment_count; ++i) {
				FramebufferColorAttachment* desc = &info.color_destriptors[i];
				glBindTexture(GL_TEXTURE_2D, this->attachments[i]);

				GLint in_format;
				GLenum format;
				GLenum type;

				if(desc->type == DATATYPE_FLOAT) {
					switch (desc->format) {
						case FORMAT_R:
							in_format = GL_R16F;
							format = GL_RED;
							break;
						case FORMAT_RG:
							in_format = GL_RG16F;
							format = GL_RG;
							break;
						case FORMAT_RGB:
							in_format = GL_RGB16F;
							format = GL_RGB;
							break;
						case FORMAT_RGBA:
							in_format = GL_RGBA16F;
							format = GL_RGBA;
							break;
						default:
							break;
					}
					type = GL_FLOAT;
				} else if (desc->type == DATATYPE_UINT8) {
					switch (desc->format) {
						case FORMAT_R:
							in_format = GL_RED;
							format = GL_RED;
							break;
						case FORMAT_RG:
							in_format = GL_RG;
							format = GL_RG;
							break;
						case FORMAT_RGB:
							in_format = GL_RGB;
							format = GL_RGB;
							break;
						case FORMAT_RGBA:
							in_format = GL_RGBA;
							format = GL_RGBA;
							break;
						default:
							break;
					}
					type = GL_UNSIGNED_BYTE;
				}

				glTexImage2D(GL_TEXTURE_2D, 0, in_format, this->width, this->height, 0, format, type, NULL);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

				color_buffers[i] = GL_COLOR_ATTACHMENT0 + i;
				glFramebufferTexture2D(GL_FRAMEBUFFER, color_buffers[i], GL_TEXTURE_2D, this->attachments[i], 0);
			}

			if(info.depth_destriptor != NULL) {
				glBindTexture(GL_TEXTURE_2D, this->depth);

				// TODO
				GLint in_format = GL_DEPTH_COMPONENT24;
				GLenum format   = GL_DEPTH_COMPONENT;
				GLenum type     = GL_FLOAT;

				glTexImage2D(GL_TEXTURE_2D, 0, in_format, this->width, this->height, 0, format, type, NULL);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depth, 0);
			}

			glDrawBuffers(this->attachment_count, color_buffers);

			GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if(status != GL_FRAMEBUFFER_COMPLETE) {
				rgLogInfo(RG_LOG_RENDER, "FRAMEBUFFER ERROR! %s", glewGetString(status));
			}
		}

		GLFramebuffer::~GLFramebuffer() {
			glDeleteTextures(this->attachment_count, this->attachments);
			glDeleteTextures(1, &this->depth);
			glDeleteFramebuffers(1, &fb);
		}

		void GLFramebuffer::Bind() {
			glViewport(0, 0, this->width, this->height);
			glBindFramebuffer(GL_FRAMEBUFFER, this->fb);
		}

		void GLFramebuffer::Unbind() {
			Engine::Math::vec2 size;
			Engine::Window::GetWindowSize(&size);
			glViewport(0, 0, size.x, size.y);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

	}
}
