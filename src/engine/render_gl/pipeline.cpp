/*
 * pipeline.cpp
 *
 *  Created on: Jul 9, 2022
 *      Author: alex9932
 */

#include <engine/render_gl/pipeline.h>

namespace Engine {
	namespace Render {

		GLPipeline::GLPipeline(const PipelineInfo& info) : Pipeline(info) {
			this->shader = (GLShader*)info.shader;
			this->fb = (GLFramebuffer*)info.framebuffer;
			this->mode = info.polygonMode;
			this->face = info.cullFace;
		}

		GLPipeline::~GLPipeline() {

		}

		void GLPipeline::Bind() {
			this->fb->Bind();
			this->shader->Start();
		}

	}
}
