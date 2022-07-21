/*
 * texture.cpp
 *
 *  Created on: Jul 17, 2022
 *      Author: alex9932
 */

#include <engine/render_vk/texture.h>
#include <engine/render_vk/vk_utils.h>

#ifdef R_VULKAN_RENDERER

namespace Engine {
	namespace Render {

		VKTexture::VKTexture(const TextureInfo& info) : Texture(info) {
		}

		VKTexture::~VKTexture() {
		}

		void VKTexture::SubData(Uint32 xoffset, Uint32 yoffset, Uint32 width, Uint32 height, void* data) {
		}

	}
}

#endif
