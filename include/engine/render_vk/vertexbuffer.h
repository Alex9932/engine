/*
 * vertexbuffer.h
 *
 *  Created on: Jul 7, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_VK_VERTEXBUFFER_H_
#define ENGINE_RENDER_VK_VERTEXBUFFER_H_

#include <engine/render/r_vertexbuffer.h>
#include <SDL2/SDL.h>

namespace Engine {
	namespace Render {

		class RG_DECLSPEC VKVertexbuffer : public Vertexbuffer {

			protected:

			public:
				VKVertexbuffer(const VertexbufferInfo& info);
				virtual ~VKVertexbuffer();
				virtual void Draw(Uint32 start, Uint32 count);
				virtual void WriteVertexbuffer(void* vertices, Uint32 offset, Uint32 size);
				virtual void WriteIndexbuffer(void* indices, Uint32 offset, Uint32 size);

		};

	}
}


#endif /* ENGINE_RENDER_GL_VERTEXBUFFER_H_ */
