/*
 * pipeline.cpp
 *
 *  Created on: Jul 9, 2022
 *      Author: alex9932
 */

#include <engine/render_gl/pipeline.h>
#include <engine/render_gl/texture.h>
#include <engine/render/window.h>

#include <engine/core/math/math.h>

namespace Engine {
	namespace Render {

		GLPipeline::GLPipeline(const PipelineInfo& info) : Pipeline(info) {
			this->shader = (GLShader*)info.shader;
			this->fb = (GLFramebuffer*)info.framebuffer;
			this->mode = info.polygonMode;
			this->face = info.cullFace;

			switch (info.polygonMode) {
				case RG_PM_FILL:
					this->gl_mode = GL_FILL;
					break;
				case RG_PM_LINE:
					this->gl_mode = GL_LINE;
					break;
				case RG_PM_POINT:
					this->gl_mode = GL_POINT;
					break;
				default:
					this->gl_mode = GL_FILL;
					break;
			}

			switch (info.cullFace) {
				case RG_CF_NONE:
					this->gl_face = GL_NONE;
					break;
				case RG_CF_FRONT:
					this->gl_face = GL_FRONT;
					break;
				case RG_CF_BACK:
					this->gl_face = GL_BACK;
					break;
				default:
					this->gl_face = GL_NONE;
					break;
			}
		}

		GLPipeline::~GLPipeline() {

		}

		void GLPipeline::Bind() {
			if(gl_face != GL_NONE) {
				glEnable(GL_CULL_FACE);
				glCullFace(gl_face);
			} else {
				glDisable(GL_CULL_FACE);
			}
			glPolygonMode(GL_FRONT_AND_BACK, gl_mode);

			if(this->fb != NULL) {
				this->fb->Bind();
			} else {
				Math::vec2 size;
				Window::GetWindowSize(&size);
				glViewport(0, 0, size.x, size.y);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
			this->shader->Start();
		}

		void GLPipeline::AttachTexture(Uint32 idx, Texture* texture) {
			glActiveTexture(GL_TEXTURE0 + idx);
			glBindTexture(GL_TEXTURE_2D, ((GLTexture*)texture)->GetGLTextureID());
		}

	}
}
