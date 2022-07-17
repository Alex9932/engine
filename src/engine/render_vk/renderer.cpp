/*
 * renderer.cpp
 *
 *  Created on: Jul 7, 2022
 *      Author: alex9932
 */

#include <engine/render/r_backend.h>

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

namespace Engine {
	namespace Render {

		static VkInstance instance;
		static VkSurfaceKHR surface;
		static VkPhysicalDevice phys_device;
		static VkDevice device;

		static VkRenderPass imgui_render_pass;

		static void MakeVKInstance() {
			VkApplicationInfo appInfo = {};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = "Vulkan application";
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = "rgEngine";
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_0;

			VkInstanceCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;

			uint32_t pCount;
			SDL_Vulkan_GetInstanceExtensions(Window::GetWindow(), &pCount, nullptr);
			std::vector<const char*> pNames(pCount);
			SDL_Vulkan_GetInstanceExtensions(Window::GetWindow(), &pCount, pNames.data());
			createInfo.enabledExtensionCount = pNames.size();
			createInfo.ppEnabledExtensionNames = pNames.data();
			createInfo.enabledLayerCount = 0;

			if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
				RG_ERROR_MSG("Failed to create Vulkan instance!");
			}

			if (!SDL_Vulkan_CreateSurface(Window::GetWindow(), instance, &surface)) {
				RG_ERROR_MSG("Failed to create surface!");
			}

			// TODO

		}

		static void MakeImguiRenderPass() {

			// TODO

		}

		void SetupBackend() {
			// TODO
		}

		void InitializeBackend() {

			RG_ERROR_MSG("Vulkan renderer is not supported yet!");

			MakeVKInstance();

			Math::vec2 size;
			Window::GetWindowSize(&size);
//			glClearColor(0, 0, 0, 1);
			ViewportBackend(0, 0, size.x, size.y);
		}

		void DestroyBackend() {
			ImGui_ImplVulkan_Shutdown();
			ImGui_ImplSDL2_Shutdown();

			vkDestroySurfaceKHR(instance, surface, nullptr);
//			vkDestroyDevice(device, nullptr);
			vkDestroyInstance(instance, nullptr);
		}

		void Swapbuffers() {
			// TODO
		}

		void ViewportBackend(float x, float y, float w, float h) {
//			glViewport(x, y, w, h);
		}

		void MakeImguiBackend() {
			MakeImguiRenderPass();

			ImGui_ImplSDL2_InitForVulkan(Window::GetWindow());


			ImGui_ImplVulkan_InitInfo info;
			info.Instance = instance;
			info.PhysicalDevice = phys_device;
			info.Device = device;

			ImGui_ImplVulkan_Init(&info, imgui_render_pass);
		}

		String GetRendererName() {
			return "Vulkan";
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

	}
}

#endif
