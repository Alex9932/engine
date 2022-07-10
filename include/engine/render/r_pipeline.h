/*
 * r_pipeline.h
 *
 *  Created on: Jul 9, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_R_PIPELINE_H_
#define ENGINE_RENDER_R_PIPELINE_H_

#include <engine/engine.h>
#include <engine/render/r_shader.h>
#include <engine/render/r_framebuffer.h>

namespace Engine {
	namespace Render {

		typedef struct PipelineInfo {
			Shader* shader;
			Framebuffer* framebuffer;
		} PipelineInfo;

		class RG_DECLSPEC Pipeline {

			protected:

			public:
				Pipeline(const PipelineInfo& info) {}
				virtual ~Pipeline() {}

				virtual void Bind() {}

		};

	}
}


#endif /* ENGINE_RENDER_R_PIPELINE_H_ */
