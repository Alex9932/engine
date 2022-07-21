/*
 * vertexbuffer.h
 *
 *  Created on: Jul 7, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_GL_VERTEXBUFFER_H_
#define ENGINE_RENDER_GL_VERTEXBUFFER_H_

#include <engine/render/r_vertexbuffer.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>

namespace Engine {
	namespace Render {

		class RG_DECLSPEC GLVertexbuffer : public Vertexbuffer {

			protected:
				GLuint vao;
				GLuint buffers[2]; // 0 - vertex buffer, 1 - index buffer
				GLuint i_size;

			public:
				GLVertexbuffer(const VertexbufferInfo& info);
				virtual ~GLVertexbuffer();
				virtual void Draw(Uint32 start, Uint32 count);
				virtual void WriteVertexbuffer(void* vertices, Uint32 offset, Uint32 size);
				virtual void WriteIndexbuffer(void* indices, Uint32 offset, Uint32 size);

		};

	}
}


#endif /* ENGINE_RENDER_GL_VERTEXBUFFER_H_ */
