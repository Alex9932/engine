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
#include <engine/render/r_texture.h>

namespace Engine {
	namespace Render {

		typedef struct FramebufferColorAttachment {
			TextureDataType type;
			TextureFormat format;
		} FramebufferColorAttachment;

		typedef struct FramebufferDepthAttachment {
			TextureDataType type;
			TextureFormat format;
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
