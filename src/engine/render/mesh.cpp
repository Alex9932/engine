/*
 * mesh.cpp
 *
 *  Created on: Feb 28, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT
#include <engine/render/mesh.h>

namespace Engine {
	namespace Render {

		static size_t vertex_size = sizeof(Vertex);
		static size_t weight_size = sizeof(Weight);

		Mesh::Mesh(Material* material, GLuint vbo, Uint32 count, Uint32* indices) {
			this->material = material;
			this->count = count;
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*)(sizeof(float) * 0)); // vXYZ
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*)(sizeof(float) * 3)); // nXYZ
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*)(sizeof(float) * 6)); // tXYZ
			glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*)(sizeof(float) * 9)); // UV
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glEnableVertexAttribArray(3);
			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Uint32) * count, indices, GL_STATIC_DRAW);
		}

		Mesh::~Mesh() {
			glDeleteBuffers(1, &ebo);
			glDeleteVertexArrays(1, &vao);
		}

		void Mesh::Draw(Shader* shader) {
			shader->SetVec4(shader->GetUniformLocation("d_color"), material->GetColor());
			shader->SetVec3(shader->GetUniformLocation("a_color"), material->GetAmbient());
			shader->SetVec3(shader->GetUniformLocation("s_color"), material->GetSpecular());
			shader->SetFloat(shader->GetUniformLocation("shininess"), material->GetShininess());
			material->Bind();
			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
		}

		Material* Mesh::GetMaterial() {
			return material;
		}



		RiggedMesh::RiggedMesh(Material* material, GLuint vbo, GLuint w_vbo, Uint32 count, Uint32* indices) {
			this->material = material;
			this->count = count;
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*)(sizeof(float) * 0)); // vXYZ
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*)(sizeof(float) * 3)); // nXYZ
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*)(sizeof(float) * 6)); // tXYZ
			glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*)(sizeof(float) * 9)); // UV
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glEnableVertexAttribArray(3);

			glBindBuffer(GL_ARRAY_BUFFER, w_vbo);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, weight_size, (GLvoid*)(sizeof(float) * 0));
			glVertexAttribIPointer(5, 4, GL_INT, weight_size, (GLvoid*)(sizeof(float) * 4));
			glEnableVertexAttribArray(4);
			glEnableVertexAttribArray(5);

			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Uint32) * count, indices, GL_STATIC_DRAW);
		}

		RiggedMesh::~RiggedMesh() {
			glDeleteBuffers(1, &ebo);
			glDeleteVertexArrays(1, &vao);
		}

		void RiggedMesh::Draw(Shader* shader) {
			shader->SetVec4(shader->GetUniformLocation("d_color"), material->GetColor());
			shader->SetVec3(shader->GetUniformLocation("a_color"), material->GetAmbient());
			shader->SetVec3(shader->GetUniformLocation("s_color"), material->GetSpecular());
			shader->SetFloat(shader->GetUniformLocation("shininess"), material->GetShininess());
			material->Bind();
			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
		}

		Material* RiggedMesh::GetMaterial() {
			return material;
		}

	}
}
