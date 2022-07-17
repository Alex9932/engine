/*
 * framebuffer.cpp
 *
 *  Created on: Jul 9, 2022
 *      Author: alex9932
 */

#include <engine/render_vk/framebuffer.h>
#include <engine/render/window.h>
#include <engine/core/math/math.h>

namespace Engine {
	namespace Render {

		VKFramebuffer::VKFramebuffer(const FramebufferInfo& info) : Framebuffer(info) {
			this->width = info.width;
			this->height = info.height;

		}

		VKFramebuffer::~VKFramebuffer() {
		}

		void VKFramebuffer::Bind() {
		}

		void VKFramebuffer::Unbind() {
		}

	}
}
