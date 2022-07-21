/*
 * vertexbuffer.cpp
 *
 *  Created on: Jul 7, 2022
 *      Author: alex9932
 */

#include <engine/render_gl/vertexbuffer.h>
#include <engine/engine.h>

namespace Engine {
	namespace Render {

		GLVertexbuffer::GLVertexbuffer(const VertexbufferInfo& info) : Vertexbuffer(info) {
			this->i_type = info.type;
			this->vertices = info.v_count;
			this->indices = info.i_count;
			this->vertex_size = info.vertex_size;

			Uint32 size = 1;
			switch (this->i_type) {
				case INDEX_UINT8:
					this->i_size = GL_UNSIGNED_BYTE;
					size = 1;
					break;
				case INDEX_UINT16:
					this->i_size = GL_UNSIGNED_SHORT;
					size = 2;
					break;
				case INDEX_UINT32:
					this->i_size = GL_UNSIGNED_INT;
					size = 4;
					break;
				default:
					this->i_size = GL_UNSIGNED_BYTE;
					size = 1;
					break;
			}

			// Vertex array object
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			// Vertex buffer
			glGenBuffers(2, this->buffers);
			glBindBuffer(GL_ARRAY_BUFFER, this->buffers[0]);
			glBufferData(GL_ARRAY_BUFFER, info.v_count * info.vertex_size, info.vertices, GL_STATIC_DRAW);

			// Index buffer
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers[1]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * info.i_count, info.indices, GL_STATIC_DRAW);

			for (Uint32 i = 0; i < info.attrib_count; ++i) {
				VertexAttribDescriptor* desc = &info.attrib_descriptors[i];
				GLenum gl_type;
				switch (desc->type) {
					case ATTRIB_BYTE:
						gl_type = GL_BYTE;
						break;
					case ATTRIB_SHORT:
						gl_type = GL_SHORT;
						break;
					case ATTRIB_INT:
						gl_type = GL_INT;
						break;
					case ATTRIB_FLOAT:
						gl_type = GL_FLOAT;
						break;
					default:
						gl_type = GL_BYTE;
						break;
				}
				if(gl_type != GL_FLOAT) {
					glVertexAttribIPointer(i, desc->elementsPerLocation, gl_type, desc->stride, (GLvoid*)(RG_CASTADDR(desc->offset)));
				} else {
					glVertexAttribPointer(i, desc->elementsPerLocation, gl_type, GL_FALSE, desc->stride, (GLvoid*)(RG_CASTADDR(desc->offset)));
				}
				glEnableVertexAttribArray(i);
			}
		}

		GLVertexbuffer::~GLVertexbuffer() {
			glDeleteBuffers(2, this->buffers);
			glDeleteVertexArrays(1, &this->vao);
		}

		void GLVertexbuffer::Draw(Uint32 start, Uint32 count) {
			glBindVertexArray(this->vao);
			glDrawElements(GL_TRIANGLES, count, this->i_size, RG_CASTADDR(start*this->i_type));
		}

		void GLVertexbuffer::WriteVertexbuffer(void* vertices, Uint32 offset, Uint32 size) {
			glBindVertexArray(this->vao);
			glBindBuffer(GL_ARRAY_BUFFER, this->buffers[0]);
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices);
		}

		void GLVertexbuffer::WriteIndexbuffer(void* indices, Uint32 offset, Uint32 size) {
			glBindVertexArray(this->vao);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers[1]);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, indices);
		}

	}
}
