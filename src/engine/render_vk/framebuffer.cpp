/*
 * framebuffer.cpp
 *
 *  Created on: Jul 9, 2022
 *      Author: alex9932
 */

#include <engine/render_vk/framebuffer.h>
#include <engine/render_vk/vk_utils.h>

#ifdef R_VULKAN_RENDERER

#include <engine/render/window.h>
#include <engine/core/math/math.h>

namespace Engine {
	namespace Render {

		void VKFramebuffer::MakeFramebuffer() {
			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = this->renderpass;
			if(customRenderpass) {
				framebufferInfo.attachmentCount = 1;
				framebufferInfo.pAttachments = &this->imageviews[0];
//				rgLogWarn(RG_LOG_RENDER, "VKDBG: Attachment: %ld", (Uint64)(this->imageviews[0]));
			} else {
				// TODO
			}
			framebufferInfo.width = this->width;
			framebufferInfo.height = this->height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(GetVkDevice(), &framebufferInfo, nullptr, &this->framebuffer) != VK_SUCCESS) {
				RG_ERROR_MSG("Failed to create framebuffer!");
			}
		}

		void VKFramebuffer::Construct(const FramebufferInfo& info) {
			this->width = info.width;
			this->height = info.height;
			this->imageview_count = info.attachment_count;

			if(customRenderpass) { return; }

			// Make renderpass
			Uint32 attchment_count = info.attachment_count;
			if(info.depth_destriptor != NULL) {
				attchment_count++;
			}

			VkAttachmentDescription attachments[attchment_count] = {};
			VkAttachmentReference references[info.attachment_count] = {};
			for (Uint32 i = 0; i < info.attachment_count; ++i) {
				VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;
				switch (info.color_destriptors[i].format) {
					case FORMAT_R:
						format = VK_FORMAT_R8_UNORM;
						break;
					case FORMAT_RG:
						format = VK_FORMAT_R8G8_UNORM;
						break;
					case FORMAT_RGB:
						format = VK_FORMAT_R8G8B8_UNORM;
						break;
					case FORMAT_RGBA:
						format = VK_FORMAT_R8G8B8A8_UNORM;
						break;
					default:
						break;
				}
				attachments[i].format = format;
				attachments[i].samples = VK_SAMPLE_COUNT_1_BIT;
				attachments[i].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				attachments[i].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				attachments[i].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachments[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachments[i].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				attachments[i].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

				references[i].attachment = i;
				references[i].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			}
			// Depth attachment
			VkAttachmentReference d_reference = {};
			if(info.depth_destriptor != NULL) {
				attachments[info.attachment_count].format = VK_FORMAT_D24_UNORM_S8_UINT;
				attachments[info.attachment_count].samples = VK_SAMPLE_COUNT_1_BIT;
				attachments[info.attachment_count].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				attachments[info.attachment_count].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachments[info.attachment_count].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachments[info.attachment_count].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachments[info.attachment_count].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				attachments[info.attachment_count].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

				d_reference.attachment = info.attachment_count;
				d_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			}

			VkSubpassDescription subpass = {};
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.colorAttachmentCount = info.attachment_count;
			subpass.pColorAttachments = references;
			if(info.depth_destriptor != NULL) {
				subpass.pDepthStencilAttachment = &d_reference;
			}
			VkSubpassDependency dependency = {};
			dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			dependency.dstSubpass = 0;
			dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.srcAccessMask = 0;
			dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			VkRenderPassCreateInfo rp_info = {};
			rp_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			rp_info.attachmentCount = attchment_count;
			rp_info.pAttachments = attachments;
			rp_info.subpassCount = 1;
			rp_info.pSubpasses = &subpass;
			rp_info.dependencyCount = 1;
			rp_info.pDependencies = &dependency;
			if(vkCreateRenderPass(GetVkDevice(), &rp_info, GetVkAllocator(), &this->renderpass) != VK_SUCCESS) {
				RG_ERROR_MSG("Failed to create renderpass!");
			}

			for (Uint32 i = 0; i < info.attachment_count; ++i) {
				// Create image
				// Create imageview
			}

			if(info.depth_destriptor != NULL) {
				// Create depth image
				// Create depth imageview
			}

		}

		VKFramebuffer::VKFramebuffer(const FramebufferInfo& info, Uint32 idx) : Framebuffer(info) {
			// Do not create renderpass, image and imageview
			this->customRenderpass = true;
			this->renderpass = GetVkRenderpass();
			this->images[0] = GetVkSwapchainImages()[idx];
			this->imageviews[0] = GetVkSwapchainImageviews()[idx];
			this->depth = NULL;
			this->depthview = NULL;
			this->Construct(info);
			this->MakeFramebuffer();
		}

		VKFramebuffer::VKFramebuffer(const FramebufferInfo& info) : Framebuffer(info) {
			this->customRenderpass = false;
			this->Construct(info);
			this->MakeFramebuffer();
		}

		VKFramebuffer::~VKFramebuffer() {
//			rgLogInfo(RG_LOG_RENDER, "~VKFramebuffer");
			if(!this->customRenderpass) {
				for (Uint32 i = 0; i < imageview_count; ++i) {
					vkDestroyImage(GetVkDevice(), this->images[i], GetVkAllocator());
					vkDestroyImageView(GetVkDevice(), this->imageviews[i], GetVkAllocator());
				}
				vkDestroyRenderPass(GetVkDevice(), this->renderpass, GetVkAllocator());
			}
			vkDestroyFramebuffer(GetVkDevice(), this->framebuffer, GetVkAllocator());
		}

		void VKFramebuffer::Bind() {
		}

		void VKFramebuffer::Unbind() {
		}

	}
}

#endif
