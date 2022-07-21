/*
 * pipeline.cpp
 *
 *  Created on: Jul 9, 2022
 *      Author: alex9932
 */

#include <engine/render_vk/pipeline.h>
#include <engine/render_vk/vk_utils.h>

#ifdef R_VULKAN_RENDERER

namespace Engine {
	namespace Render {

		VKPipeline::VKPipeline(const PipelineInfo& info) : Pipeline(info) {
		}

		VKPipeline::~VKPipeline() {

		}

		void VKPipeline::Bind() {
		}

		void VKPipeline::AttachTexture(Uint32 idx, Texture* texture) {
		}

	}
}

#endif
