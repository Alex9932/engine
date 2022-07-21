/*
 * r_vertexbuffer.h
 *
 *  Created on: Jul 7, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_R_VERTEXBUFFER_H_
#define ENGINE_RENDER_R_VERTEXBUFFER_H_

#include <SDL2/SDL.h>
#include <engine/engine.h>

namespace Engine {
	namespace Render {

		enum IndexType {
			INDEX_UINT8  = 1, // NOT RECOMENDED TO USE (USE UINT16 OR UINT32)
			INDEX_UINT16 = 2,
			INDEX_UINT32 = 4
		};

		enum AttribType {
			ATTRIB_BYTE = 0,
			ATTRIB_SHORT,
			ATTRIB_INT,
			ATTRIB_FLOAT
		};

		typedef struct VertexAttribDescriptor {
			Uint32 elementsPerLocation;
			Uint32 stride;
			Uint32 offset;
			AttribType type;
		} VertexAttribDescriptor;

		typedef struct VertexbufferInfo {
			Uint32 vertex_size;
			IndexType type;
			Uint32 v_count;
			Uint32 i_count;
			void* vertices;
			void* indices;
			Uint32 attrib_count;
			VertexAttribDescriptor* attrib_descriptors;
		} VertexbufferInfo;

		class RG_DECLSPEC Vertexbuffer {

			protected:
				IndexType i_type   = INDEX_UINT8;
				Uint32 vertices    = 0;
				Uint32 indices     = 0;
				Uint32 vertex_size = 0;

			public:
				Vertexbuffer(const VertexbufferInfo& info) {}
				virtual ~Vertexbuffer() {}
				virtual void Draw(Uint32 start, Uint32 count) {}
				virtual void WriteVertexbuffer(void* vertices, Uint32 offset, Uint32 size) {}
				virtual void WriteIndexbuffer(void* indices, Uint32 offset, Uint32 size)    {}

		};

	}
}


#endif /* ENGINE_RENDER_R_VERTEXBUFFER_H_ */
