/*
 * framebuffer.h
 *
 *  Created on: Jul 9, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_GL_FRAMEBUFFER_H_
#define ENGINE_RENDER_GL_FRAMEBUFFER_H_

#include <engine/render/r_framebuffer.h>
#include <GL/glew.h>

#define RG_GL_FBATTACHMENTS 16

namespace Engine {
	namespace Render {

		class RG_DECLSPEC GLFramebuffer : public Framebuffer {

			protected:
				GLuint fb;
				GLuint attachments[RG_GL_FBATTACHMENTS];
				GLuint depth;
				Uint32 attachment_count;

			public:
				GLFramebuffer(const FramebufferInfo& info);
				virtual ~GLFramebuffer();

				void Bind();
				void Unbind();

		};

	}
}


#endif /* ENGINE_RENDER_GL_FRAMEBUFFER_H_ */
