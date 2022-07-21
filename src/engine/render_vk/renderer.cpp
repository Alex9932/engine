/*
 * renderer.cpp
 *
 *  Created on: Jul 7, 2022
 *      Author: alex9932
 */

#include <engine/render/r_backend.h>
#include <engine/render_vk/vk_utils.h>

#ifdef R_VULKAN_RENDERER

#include <engine/render/window.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include <imgui_impl_vulkan.h>
#include <imgui_impl_sdl.h>

#include <engine/render_vk/vertexbuffer.h>
#include <engine/render_vk/framebuffer.h>
#include <engine/render_vk/shader.h>
#include <engine/render_vk/pipeline.h>
#include <engine/render_vk/texture.h>

#define FRAMES 2

namespace Engine {
	namespace Render {

		static char description[256];
		static VkRenderPass renderpass;
		static VKFramebuffer* framebuffer[FRAMES] = {};
		static bool rebuildSwapChain = false;

		static Uint32 frame = 0;
		static Uint32 framecount = FRAMES;

	    // Vulkan fields
		static VkAllocationCallbacks* vk_allocator = NULL;
	    static Uint32 extensions_count;
	    static String* extensions;
		static VkInstance instance;
		static VkPhysicalDevice phys_device;
		static VkDevice device;
		static Sint32 queueFamily = -1;
		static Sint32 presentFamily;
		static VkQueue queue;
		static VkDescriptorPool descriptorPool;
		static VkSurfaceKHR surface;
		static VkSwapchainKHR swapchain;
		static VkExtent2D swapchain_extent;

		static VkFormat swapchain_format = VK_FORMAT_B8G8R8A8_SRGB;

		static VkSemaphore semaphores[FRAMES];
		static VkSemaphore render_semaphores[FRAMES];
		static VkFence fences[FRAMES];

		static VkImage images[FRAMES];
		static VkImageView imageviews[FRAMES];

		static VkCommandPool commandPool;
		static VkCommandBuffer commandbuffers[FRAMES];

		static void MakeVKInstance() {
			// Get extensions
		    SDL_Vulkan_GetInstanceExtensions(Window::GetWindow(), &extensions_count, NULL);
		    extensions = new String[extensions_count];
		    SDL_Vulkan_GetInstanceExtensions(Window::GetWindow(), &extensions_count, extensions);
		    rgLogInfo(RG_LOG_RENDER, "Vulkan extensions: %d", extensions_count);

			VkApplicationInfo appInfo = {};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = GetGame()->GetName();
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = "rgEngine";
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_0;

			VkInstanceCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;
			createInfo.enabledExtensionCount = extensions_count;
			createInfo.ppEnabledExtensionNames = extensions;
#ifdef RG_GRAPHICSAPI_DEBUG
			std::vector<const char*> layerNames;
			layerNames.push_back("VK_LAYER_KHRONOS_validation");
			createInfo.enabledLayerCount = layerNames.size();
			createInfo.ppEnabledLayerNames = layerNames.data();
#else
			createInfo.enabledLayerCount = 0;
#endif

			if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
				RG_ERROR_MSG("Failed to create Vulkan instance!");
			}
		}

		static void SelectGPU() {
			Uint32 gpu_count;
			if (vkEnumeratePhysicalDevices(instance, &gpu_count, NULL) != VK_SUCCESS) {
				RG_ERROR_MSG("Failed to enumerate physical devices!");
			}

			VkPhysicalDevice* gpus = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * gpu_count);
			vkEnumeratePhysicalDevices(instance, &gpu_count, gpus);

			// If a number >1 of GPUs got reported, find discrete GPU if present, or use first one available. This covers
			// most common cases (multi-gpu/integrated+dedicated graphics). Handling more complicated setups (multiple
			// dedicated GPUs) is out of scope of this sample.
			int use_gpu = 0;
			for (int i = 0; i < (int)gpu_count; i++) {
				VkPhysicalDeviceProperties properties;
				vkGetPhysicalDeviceProperties(gpus[i], &properties);
				if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
					use_gpu = i;
					break;
				}
			}

			phys_device = gpus[use_gpu];
			free(gpus);

			// Get device information
			VkPhysicalDeviceProperties properties;
			vkGetPhysicalDeviceProperties(phys_device, &properties);
			sprintf(description, "%s", properties.deviceName);
		}

		static void SelectGraphicsQueueFamily() {
			Uint32 count;
			vkGetPhysicalDeviceQueueFamilyProperties(phys_device, &count, NULL);
			VkQueueFamilyProperties* queues = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * count);
			vkGetPhysicalDeviceQueueFamilyProperties(phys_device, &count, queues);
			for (Uint32 i = 0; i < count; i++) {
				if (queueFamily == -1 && (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
					queueFamily = i;
				}
				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(phys_device, i, surface, &presentSupport);
				if (presentSupport) {
					presentFamily = i;
					break;
				}
			}
			free(queues);
			//RG_ASSERT(queueFamily != (uint32_t)-1);
		}

		static void CreateLogicalDevice() {
			int device_extension_count = 1;
			const char* device_extensions[] = { "VK_KHR_swapchain" };
			const float queue_priority[] = { 1.0f };
			VkDeviceQueueCreateInfo queue_info[1] = {};
			queue_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queue_info[0].queueFamilyIndex = queueFamily;
			queue_info[0].queueCount = 1;
			queue_info[0].pQueuePriorities = queue_priority;
			VkDeviceCreateInfo create_info = {};
			create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			create_info.queueCreateInfoCount = sizeof(queue_info) / sizeof(queue_info[0]);
			create_info.pQueueCreateInfos = queue_info;
			create_info.enabledExtensionCount = device_extension_count;
			create_info.ppEnabledExtensionNames = device_extensions;
			if(vkCreateDevice(phys_device, &create_info, vk_allocator, &device) != VK_SUCCESS) {
				RG_ERROR_MSG("Failed to create logical device!");
			}
			vkGetDeviceQueue(device, queueFamily, 0, &queue);

			//
		}

		static void CreateDescriptorPool() {
			VkDescriptorPoolSize pool_sizes[] = {
				{ VK_DESCRIPTOR_TYPE_SAMPLER,                1000 },
				{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
				{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,          1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,          1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,   1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,   1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
				{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,       1000 }
			};
			VkDescriptorPoolCreateInfo pool_info = {};
			pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
			pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
			pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
			pool_info.pPoolSizes = pool_sizes;
			if(vkCreateDescriptorPool(device, &pool_info, vk_allocator, &descriptorPool) != VK_SUCCESS) {
				RG_ERROR_MSG("Failed to create descriptor pool!");
			}
		}

		static void CreateSwapchain() {
			// TODO
			VkSurfaceFormatKHR surfaceFormat = {};
			surfaceFormat.format = swapchain_format;
			surfaceFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

			VkSurfaceCapabilitiesKHR capabilities = {};
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(phys_device, surface, &capabilities);

			VkSwapchainCreateInfoKHR info = {};
			info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			info.surface = surface;
			info.minImageCount = framecount;
			info.imageFormat = surfaceFormat.format;
			info.imageColorSpace = surfaceFormat.colorSpace;
			info.imageExtent = capabilities.currentExtent;
			info.imageArrayLayers = 1;
			info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			if (queueFamily != presentFamily) {
				rgLogWarn(RG_LOG_RENDER, "VK: Using different families");
				Uint32 queueFamilyIndices[] = {(Uint32)queueFamily, (Uint32)presentFamily};
				info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
				info.queueFamilyIndexCount = 2;
				info.pQueueFamilyIndices = queueFamilyIndices;
			} else {
				rgLogWarn(RG_LOG_RENDER, "VK: Using one family");
				info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
				info.queueFamilyIndexCount = 0; // Optional
				info.pQueueFamilyIndices = nullptr; // Optional
			}
			info.preTransform = capabilities.currentTransform;
			info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			info.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR; // TODO
			info.clipped = VK_TRUE;
			info.oldSwapchain = VK_NULL_HANDLE;
			if (vkCreateSwapchainKHR(device, &info, vk_allocator, &swapchain) != VK_SUCCESS) {
				RG_ERROR_MSG("failed to create swap chain!");
			}

			Uint32 imageCount;
			vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images);

			rgLogInfo(RG_LOG_RENDER, "VKDBG: Swapchain images: %d, must be: %d", imageCount, info.minImageCount);

			for (Uint32 i = 0; i < imageCount; ++i) {
				VkImageViewCreateInfo viewInfo = {};
				viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				viewInfo.image = images[i];
				viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				viewInfo.format = surfaceFormat.format;
				viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				viewInfo.subresourceRange.baseMipLevel = 0;
				viewInfo.subresourceRange.levelCount = 1;
				viewInfo.subresourceRange.baseArrayLayer = 0;
				viewInfo.subresourceRange.layerCount = 1;
				if (vkCreateImageView(device, &viewInfo, vk_allocator, &imageviews[i]) != VK_SUCCESS) {
					RG_ERROR_MSG("failed to create texture image view!");
				}
//				rgLogInfo(RG_LOG_RENDER, "VKDBG: Created imageview: %ld", (Uint64)imageviews[i]);
			}
		}

		static void MakeRenderPass() {
	        VkAttachmentDescription attachment = {};
	        attachment.format = swapchain_format;
	        attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	        attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	        attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	        attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	        attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	        attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	        attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	        VkAttachmentReference color_attachment = {};
	        color_attachment.attachment = 0;
	        color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	        VkSubpassDescription subpass = {};
	        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	        subpass.colorAttachmentCount = 1;
	        subpass.pColorAttachments = &color_attachment;
	        VkSubpassDependency dependency = {};
	        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	        dependency.dstSubpass = 0;
	        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	        dependency.srcAccessMask = 0;
	        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	        VkRenderPassCreateInfo info = {};
	        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	        info.attachmentCount = 1;
	        info.pAttachments = &attachment;
	        info.subpassCount = 1;
	        info.pSubpasses = &subpass;
	        info.dependencyCount = 1;
	        info.pDependencies = &dependency;
	        if(vkCreateRenderPass(device, &info, vk_allocator, &renderpass) != VK_SUCCESS) {
	        	RG_ERROR_MSG("Failed to create renderpass!");
	        }
		}

		static void ResizeFramebuffer() {
			if(framebuffer[0] != NULL) {
				for(Uint32 i = 0; i < FRAMES; i++) {
					framebuffer[i]->~Framebuffer();
				}
				Uint32 imageCount;
				vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images);
				for (Uint32 i = 0; i < imageCount; ++i) {
					vkDestroyImageView(device, imageviews[i], vk_allocator);
				}
				vkDestroySwapchainKHR(device, swapchain, vk_allocator);
			} else {
				for(Uint32 i = 0; i < FRAMES; i++) {
					framebuffer[i] = (VKFramebuffer*)malloc(sizeof(VKFramebuffer));
				}
			}

			CreateSwapchain();
			Math::vec2 size;
			Window::GetWindowSize(&size);
			swapchain_extent.width = size.x;
			swapchain_extent.height = size.y;
			FramebufferColorAttachment fb_attachment;
			fb_attachment.type = DATATYPE_UINT8;
			fb_attachment.format = FORMAT_RGBA;
			FramebufferInfo fb_info;
			fb_info.width = size.x;
			fb_info.height = size.y;
			fb_info.attachment_count = 1;
			fb_info.color_destriptors = &fb_attachment;
			for(Uint32 i = 0; i < FRAMES; i++) {
				framebuffer[i] = new(framebuffer[i]) VKFramebuffer(fb_info, i);
			}
		}

		static void BeginFrame() {
			vkWaitForFences(device, 1, &fences[frame], VK_TRUE, UINT64_MAX);
			vkResetFences(device, 1, &fences[frame]);

//			rgLogInfo(RG_LOG_RENDER, "VKDBG: AcquireNextImage");
			uint32_t imageIndex;
			VkResult result = vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, semaphores[frame], VK_NULL_HANDLE, &imageIndex);

			if (result == VK_ERROR_OUT_OF_DATE_KHR) {
				rebuildSwapChain = true;
//				return;
			}

			vkResetCommandBuffer(commandbuffers[frame], 0);

			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			if (vkBeginCommandBuffer(commandbuffers[frame], &beginInfo) != VK_SUCCESS) {
				RG_ERROR_MSG("failed to begin recording command buffer!");
			}

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = renderpass;
			renderPassInfo.framebuffer = framebuffer[frame]->GetFramebuffer();

			renderPassInfo.renderArea.offset = {0, 0};
			renderPassInfo.renderArea.extent = swapchain_extent;

		//	VkClearValue clearColor = {{{0.0f, 0.0f, SDL_sqrtf(SDL_powf(SDL_sin(upTime), 2)), 1.0f}}};
			VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};

			VkClearValue clearValues[] = {clearColor};
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = clearValues;


			vkCmdBeginRenderPass(commandbuffers[frame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		}

		static void EndFrame() {
			vkCmdEndRenderPass(commandbuffers[frame]);

			if (vkEndCommandBuffer(commandbuffers[frame]) != VK_SUCCESS) {
				RG_ERROR_MSG("failed to record command buffer!");
			}


			VkSubmitInfo submitInfo{};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

			VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = &semaphores[frame];
			submitInfo.pWaitDstStageMask = waitStages;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandbuffers[frame];
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = &render_semaphores[frame];
			vkQueueSubmit(queue, 1, &submitInfo, fences[frame]);

			// TODO
			VkPresentInfoKHR info = {};

			info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			info.waitSemaphoreCount = 1;
			info.pWaitSemaphores = &render_semaphores[frame];
			info.swapchainCount = 1;
			info.pSwapchains = &swapchain;
			info.pImageIndices = &frame;

//			rgLogInfo(RG_LOG_RENDER, "VKDBG: Present frame: %d", frame);
			VkResult err = vkQueuePresentKHR(queue, &info);
			if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
				rebuildSwapChain = true;
//				return;
			}
		}

		void SetupBackend() {}

		void InitializeBackend() {

			MakeVKInstance();
			SelectGPU();
			if (!SDL_Vulkan_CreateSurface(Window::GetWindow(), instance, &surface)) {
				RG_ERROR_MSG("Failed to create Vulkan surface!");
			}

//			RG_ERROR_MSG("Vulkan renderer is not supported yet!");

			SelectGraphicsQueueFamily();
			CreateLogicalDevice();
			CreateDescriptorPool();

			VkCommandPoolCreateInfo poolInfo = {};
			poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			poolInfo.queueFamilyIndex = queueFamily;
			if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
				RG_ERROR_MSG("failed to create command pool!");
			}

			for (Uint32 i = 0; i < FRAMES; ++i) {
				VkSemaphoreCreateInfo semaphoreInfo = {};
				semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
				VkFenceCreateInfo fenceInfo = {};
				fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
				fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
				vkCreateSemaphore(device, &semaphoreInfo, vk_allocator, &semaphores[i]);
				vkCreateSemaphore(device, &semaphoreInfo, vk_allocator, &render_semaphores[i]);
				vkCreateFence(device, &fenceInfo, vk_allocator, &fences[i]);

				VkCommandBufferAllocateInfo allocInfo{};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.commandPool = commandPool;
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandBufferCount = FRAMES;

				if (vkAllocateCommandBuffers(device, &allocInfo, commandbuffers) != VK_SUCCESS) {
					RG_ERROR_MSG("failed to allocate command buffers!");
				}
			}

			MakeRenderPass();
			ResizeFramebuffer();
			Math::vec2 size;
			Window::GetWindowSize(&size);
			ViewportBackend(0, 0, size.x, size.y);

			BeginFrame();

			rgLogInfo(RG_LOG_RENDER, "Vulkan: %s", description);
			rgLogInfo(RG_LOG_RENDER, "Initialized Vulkan rendering backend");
		}

		void DestroyBackend() {
			EndFrame();

			ImGui_ImplVulkan_Shutdown();
			ImGui_ImplSDL2_Shutdown(); // @suppress("Invalid arguments")


			for (Uint32 i = 0; i < FRAMES; ++i) {
				vkFreeCommandBuffers(device, commandPool, 1, &commandbuffers[i]);
				vkDestroySemaphore(device, semaphores[i], vk_allocator);
				vkDestroySemaphore(device, render_semaphores[i], vk_allocator);
				vkDestroyFence(device, fences[i], vk_allocator);
			}
			vkDestroyCommandPool(device, commandPool, vk_allocator);

			vkDestroyRenderPass(device, renderpass, vk_allocator);

			for(Uint32 i = 0; i < FRAMES; i++) {
				framebuffer[i]->~Framebuffer();
				free(framebuffer[i]);
			}

			vkDestroyDescriptorPool(device, descriptorPool, vk_allocator);
			vkDestroySwapchainKHR(device, swapchain, vk_allocator);
			vkDestroySurfaceKHR(instance, surface, vk_allocator);
			vkDestroyDevice(device, vk_allocator);
			vkDestroyInstance(instance, vk_allocator);

			delete[] extensions;
		}

		static void PresentFrame() {
			if (rebuildSwapChain) { return; }

			EndFrame();

			frame = (frame + 1) % framecount;

			BeginFrame();
		}

		void Swapbuffers() {
			PresentFrame();
//			rgLogInfo(RG_LOG_RENDER, "VK: Swapbuffers");

			if(rebuildSwapChain) {
				rgLogInfo(RG_LOG_RENDER, "VK: Resize");
				ResizeFramebuffer();
				rebuildSwapChain = false;
			}
		}

		void ViewportBackend(float x, float y, float w, float h) {
//			glViewport(x, y, w, h);
		}

		void MakeImguiBackend() {
			ImGui_ImplSDL2_InitForVulkan(Window::GetWindow()); // @suppress("Invalid arguments")
			ImGui_ImplVulkan_InitInfo info;
			SDL_memset(&info, 0, sizeof(ImGui_ImplVulkan_InitInfo));
			info.Instance = instance;
			info.PhysicalDevice = phys_device;
			info.Device = device;
			info.QueueFamily = queueFamily;
			info.Queue = queue;
			info.PipelineCache = NULL; // TODO
			info.DescriptorPool = descriptorPool;
			info.Subpass = 0; // TODO
			info.MinImageCount = FRAMES;
			info.ImageCount = FRAMES;
			info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
			info.Allocator = vk_allocator;
			info.CheckVkResultFn = NULL; // TODO
			ImGui_ImplVulkan_Init(&info, renderpass);

			// Make font
			VkCommandPool commandPool;
			VkCommandPoolCreateInfo poolInfo = {};
			poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			poolInfo.queueFamilyIndex = queueFamily;
			if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
				RG_ERROR_MSG("failed to create command pool!");
			}

	        VkCommandBuffer command_buffer;
	        VkCommandBufferAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.commandPool = commandPool;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandBufferCount = 1;
			if (vkAllocateCommandBuffers(device, &allocInfo, &command_buffer) != VK_SUCCESS) {
				RG_ERROR_MSG("failed to allocate command buffers!");
			}

	        vkResetCommandPool(device, commandPool, 0);
	        VkCommandBufferBeginInfo begin_info = {};
	        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	        vkBeginCommandBuffer(command_buffer, &begin_info);

	        ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

	        VkSubmitInfo end_info = {};
	        end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	        end_info.commandBufferCount = 1;
	        end_info.pCommandBuffers = &command_buffer;
	        vkEndCommandBuffer(command_buffer);
	        vkQueueSubmit(queue, 1, &end_info, VK_NULL_HANDLE);
	        vkDeviceWaitIdle(device);
	        ImGui_ImplVulkan_DestroyFontUploadObjects();

	        vkFreeCommandBuffers(device, commandPool, 1, &command_buffer);
	        vkDestroyCommandPool(device, commandPool, vk_allocator);
		}

		String GetRendererName() {
			return "Vulkan";
		}

		String GetRendererDescription() {
			return description;
		}

		Framebuffer* GetDefaultFramebuffer() {
			return framebuffer[frame];
		}

		// Objects
		Vertexbuffer* MakeVertexbuffer(const VertexbufferInfo& info, Allocator* allocator) {
			return new(allocator->Allocate(sizeof(VKVertexbuffer))) VKVertexbuffer(info);
		}

		void DeleteVertexbuffer(Vertexbuffer* vertexbuffer, Allocator* allocator) {
			vertexbuffer->~Vertexbuffer();
			allocator->Deallocate(vertexbuffer);
		}

		Framebuffer* MakeFramebuffer(const FramebufferInfo& info, Allocator* allocator) {
			return new(allocator->Allocate(sizeof(VKFramebuffer))) VKFramebuffer(info);
		}

		void DeleteFramebuffer(Framebuffer* framebuffer, Allocator* allocator) {
			framebuffer->~Framebuffer();
			allocator->Deallocate(framebuffer);
		}

		Pipeline* MakePipeline(const PipelineInfo& info, Allocator* allocator) {
			return new(allocator->Allocate(sizeof(VKPipeline))) VKPipeline(info);
		}

		void DeletePipeline(Pipeline* pipeline, Allocator* allocator) {
			pipeline->~Pipeline();
			allocator->Deallocate(pipeline);
		}

		Shader* MakeShader(const ShaderInfo& info, Allocator* allocator) {
			return new(allocator->Allocate(sizeof(VKShader))) VKShader(info);
		}

		void DeleteShader(Shader* shader, Allocator* allocator) {
			shader->~Shader();
			allocator->Deallocate(shader);
		}

		Texture* MakeTexture(const TextureInfo& info, Allocator* allocator) {
			return new(allocator->Allocate(sizeof(VKTexture))) VKTexture(info);
		}

		void DeleteTexture(Texture* texture, Allocator* allocator) {
			texture->~Texture();
			allocator->Deallocate(texture);
		}

		// vk_utils.h
		VkInstance GetVkInstance() {
			return instance;
		}

		VkPhysicalDevice GetVkPhysicalDevice() {
			return phys_device;
		}

		VkDevice GetVkDevice() {
			return device;
		}

		VkAllocationCallbacks* GetVkAllocator() {
			return vk_allocator;
		}

		VkFormat GetVkSwapchainFormat() {
			return swapchain_format;
		}

		VkCommandBuffer GetVkCommandBuffer() {
			return commandbuffers[frame];
		}

		VkRenderPass GetVkRenderpass() {
			return renderpass;
		}

		VkImage* GetVkSwapchainImages() {
			return images;
		}

		VkImageView* GetVkSwapchainImageviews() {
			return imageviews;
		}

	}
}

#endif
