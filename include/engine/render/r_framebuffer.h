/*
 * r_framebuffer.h
 *
 *  Created on: Jul 7, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_R_FRAMEBUFFER_H_
#define ENGINE_RENDER_R_FRAMEBUFFER_H_

#include <SDL2/SDL.h>
#include <engine/engine.h>

namespace Engine {
	namespace Render {

		enum AttachmentType {
			ATTACHMENT_COLOR = 0,
			ATTACHMENT_DEPTH
		};

		enum AttachmentFormat {
			ATTACHMENT_R = 0,
			ATTACHMENT_RG,
			ATTACHMENT_RGB,
			ATTACHMENT_RGBA,
			ATTACHMENT_DEPTH24
		};

		typedef struct FramebufferColorAttachment {
			AttachmentType type;
			AttachmentFormat format;
		} FramebufferColorAttachment;

		typedef struct FramebufferDepthAttachment {
			AttachmentType type;
			AttachmentFormat format;
		} FramebufferDepthAttachment;

		typedef struct FramebufferInfo {
			Uint32 width;
			Uint32 height;
			Uint32 attachment_count;
			FramebufferColorAttachment* color_destriptors = NULL;
			FramebufferDepthAttachment* depth_destriptor  = NULL;
		} FramebufferInfo;

		class Framebuffer {

			protected:
				Uint32 width  = 0;
				Uint32 height = 0;

			public:
				Framebuffer(const FramebufferInfo& info) {}
				virtual ~Framebuffer() {}

		};

	}
}


#endif /* ENGINE_RENDER_R_FRAMEBUFFER_H_ */
