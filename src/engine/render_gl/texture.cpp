/*
 * texture.cpp
 *
 *  Created on: Jul 17, 2022
 *      Author: alex9932
 */

#include <engine/render_gl/texture.h>

namespace Engine {
	namespace Render {

		GLTexture::GLTexture(const TextureInfo& info) : Texture(info) {

			GLint in_format;
			if(info.type == DATATYPE_FLOAT) {
				switch (info.format) {
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
			} else if (info.type == DATATYPE_UINT8) {
				switch (info.format) {
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

			GLint filter;
			switch (info.filter) {
				case FILTER_NEAREST:
					filter = GL_NEAREST;
					break;
				case FILTER_LINEAR:
					filter = GL_LINEAR;
					break;
				default:
					break;
			}

			glGenTextures(1, &this->texture);
			glBindTexture(GL_TEXTURE_2D, this->texture);
			glTexImage2D(GL_TEXTURE_2D, 0, in_format, info.width, info.height, 0, format, type, info.data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

		}

		GLTexture::~GLTexture() {
			glDeleteTextures(1, &this->texture);
		}

		void GLTexture::SubData(Uint32 xoffset, Uint32 yoffset, Uint32 width, Uint32 height, void* data) {
			glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, width, height, format, type, data);
		}

	}
}
