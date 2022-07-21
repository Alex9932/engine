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

		enum PolygonMode {
			RG_PM_FILL = 0,
			RG_PM_LINE,
			RG_PM_POINT
		};

		enum CullFace {
			RG_CF_NONE = 0,
			RG_CF_FRONT,
			RG_CF_BACK
		};

		typedef struct PipelineInfo {
			Shader* shader;
			Framebuffer* framebuffer;
			PolygonMode polygonMode;
			CullFace cullFace;

		} PipelineInfo;

		class RG_DECLSPEC Pipeline {

			protected:
				PolygonMode mode = RG_PM_FILL;
				CullFace face    = RG_CF_NONE;

			public:
				Pipeline(const PipelineInfo& info) {}
				virtual ~Pipeline() {}

				virtual void Bind() {}
				virtual void AttachTexture(Uint32 idx, Texture* texture) {}

		};

	}
}


#endif /* ENGINE_RENDER_R_PIPELINE_H_ */
