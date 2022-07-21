/*
 * vk_utils.h
 *
 *  Created on: Jul 20, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_VK_VK_UTILS_H_
#define ENGINE_RENDER_VK_VK_UTILS_H_

#include <engine/render/r_backend.h>
#include <vulkan/vulkan.h>

#ifdef R_VULKAN_RENDERER

namespace Engine {
	namespace Render {

		VkInstance GetVkInstance();
		VkPhysicalDevice GetVkPhysicalDevice();
		VkDevice GetVkDevice();
		VkAllocationCallbacks* GetVkAllocator();
		VkFormat GetVkSwapchainFormat();

		VkCommandBuffer GetVkCommandBuffer();

		VkRenderPass GetVkRenderpass();
		VkImage* GetVkSwapchainImages();
		VkImageView* GetVkSwapchainImageviews();

	}
}

#endif

#endif /* ENGINE_RENDER_VK_VK_UTILS_H_ */
