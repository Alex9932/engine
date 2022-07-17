/*
 * pipeline.h
 *
 *  Created on: Jul 9, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_VK_PIPELINE_H_
#define ENGINE_RENDER_VK_PIPELINE_H_

#include <engine/engine.h>
#include <engine/render/r_pipeline.h>
#include <vulkan/vulkan.h>

namespace Engine {
	namespace Render {

		class RG_DECLSPEC VKPipeline : public Pipeline {

			protected:
				VkRenderPass renderpass;

			public:
				VKPipeline(const PipelineInfo& info);
				~VKPipeline();

				virtual void Bind();

				RG_INLINE VkRenderPass GetRenderPass() {
					return renderpass;
				}

		};

	}
}


#endif /* ENGINE_RENDER_GL_PIPELINE_H_ */
