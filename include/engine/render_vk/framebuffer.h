/*
 * framebuffer.h
 *
 *  Created on: Jul 9, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_VK_FRAMEBUFFER_H_
#define ENGINE_RENDER_VK_FRAMEBUFFER_H_

#include <engine/render/r_framebuffer.h>

namespace Engine {
	namespace Render {

		class RG_DECLSPEC VKFramebuffer : public Framebuffer {

			protected:

			public:
				VKFramebuffer(const FramebufferInfo& info);
				virtual ~VKFramebuffer();

				void Bind();
				void Unbind();

		};

	}
}


#endif /* ENGINE_RENDER_GL_FRAMEBUFFER_H_ */
