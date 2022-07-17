/*
 * pipeline.cpp
 *
 *  Created on: Jul 9, 2022
 *      Author: alex9932
 */

#include <engine/render_vk/pipeline.h>

namespace Engine {
	namespace Render {

		VKPipeline::VKPipeline(const PipelineInfo& info) : Pipeline(info) {
		}

		VKPipeline::~VKPipeline() {

		}

		void VKPipeline::Bind() {
		}

	}
}
