/*
 * framebuffer.h
 *
 *  Created on: Jul 9, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_VK_FRAMEBUFFER_H_
#define ENGINE_RENDER_VK_FRAMEBUFFER_H_

#include <engine/render/r_framebuffer.h>
#include <vulkan/vulkan.h>

#define RG_VK_IMAGES 16

namespace Engine {
	namespace Render {

		class RG_DECLSPEC VKFramebuffer : public Framebuffer {

			protected:
				VkRenderPass renderpass;
				VkImage images[RG_VK_IMAGES];
				VkImageView imageviews[RG_VK_IMAGES];
				VkImage depth;
				VkImageView depthview;
				VkFramebuffer framebuffer;
				Uint32 imageview_count;
				bool customRenderpass;

				void MakeFramebuffer();
				void Construct(const FramebufferInfo& info);

			public:
				VKFramebuffer(const FramebufferInfo& info);
				VKFramebuffer(const FramebufferInfo& info, Uint32 idx); // Do not use in your application
				virtual ~VKFramebuffer();

				void Bind();
				void Unbind();

				RG_INLINE VkRenderPass GetRenderPass() { return renderpass; }
				RG_INLINE VkImageView GetImageView(Uint32 i) { return imageviews[i]; }
				RG_INLINE VkImageView GetDepthImageView() { return depthview; }
				RG_INLINE VkImage GetImage(Uint32 i) { return images[i]; }
				RG_INLINE VkImage GetDepthImage() { return depth; }
				RG_INLINE VkFramebuffer GetFramebuffer() { return framebuffer; }

		};

	}
}


#endif /* ENGINE_RENDER_GL_FRAMEBUFFER_H_ */
