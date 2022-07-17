/*
 * vertexbuffer.cpp
 *
 *  Created on: Jul 7, 2022
 *      Author: alex9932
 */

#include <engine/render_vk/vertexbuffer.h>
#include <engine/engine.h>

namespace Engine {
	namespace Render {

		VKVertexbuffer::VKVertexbuffer(const VertexbufferInfo& info) : Vertexbuffer(info) {
			this->i_type = info.type;
			this->vertices = info.v_count;
			this->indices = info.i_count;
			this->vertex_size = info.vertex_size;
		}

		VKVertexbuffer::~VKVertexbuffer() {
		}

		void VKVertexbuffer::Draw(Uint32 start, Uint32 count) {
		}

		void VKVertexbuffer::WriteVertexbuffer(float* vertices, Uint32 offset, Uint32 size) {
		}

		void VKVertexbuffer::WriteIndexbuffer(void* indices, Uint32 offset, Uint32 size) {
		}

	}
}
