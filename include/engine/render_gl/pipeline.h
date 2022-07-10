/*
 * pipeline.h
 *
 *  Created on: Jul 9, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_GL_PIPELINE_H_
#define ENGINE_RENDER_GL_PIPELINE_H_

#include <engine/engine.h>
#include <engine/render/r_pipeline.h>

#include <engine/render_gl/shader.h>
#include <engine/render_gl/framebuffer.h>

namespace Engine {
	namespace Render {

		class RG_DECLSPEC GLPipeline : public Pipeline {

			protected:
				GLShader* shader;
				GLFramebuffer* fb;

			public:
				GLPipeline(const PipelineInfo& info);
				~GLPipeline();

				virtual void Bind();

		};

	}
}


#endif /* ENGINE_RENDER_GL_PIPELINE_H_ */
