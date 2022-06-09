/*
 * model.cpp
 *
 *  Created on: Feb 28, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/render/model.h>
#include <engine/render/render.h>

namespace Engine {
	namespace Render {

		static size_t vertex_size = sizeof(Vertex);
		static size_t weight_size = sizeof(Weight);

		Model::Model(Geometry* geom) {
			mdl_vertices = (Vertex*)GetGeomAllocator()->Allocate(sizeof(Vertex) * geom->vertex_count);
			memcpy(mdl_vertices, geom->vertices, sizeof(Vertex) * geom->vertex_count);
			vertices = geom->vertex_count;

			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, geom->vertex_count * vertex_size, geom->vertices, GL_STATIC_DRAW);
			Uint32 offset = 0;
			for (Uint32 i = 0; i < geom->meshes; ++i) {
				meshes.push_back(new Mesh(geom->materials[i], vbo, geom->indices_count[i], &geom->indices[offset]));
				offset += geom->indices_count[i];
			}
		}

		Model::~Model() {
			GetGeomAllocator()->Deallocate(mdl_vertices);
			glDeleteBuffers(1, &vbo);
			for(Uint32 i = 0; i < meshes.size(); i++) {
				delete meshes[i];
			}
		}

		void Model::Draw(Shader* shader) {
			for (Uint32 i = 0; i < meshes.size(); ++i) {
				meshes[i]->Draw(shader);
			}
		}

		std::vector<Mesh*>& Model::GetMeshes() {
			return meshes;
		}

		GLuint Model::GetVBO() {
			return vbo;
		}

		Vertex* Model::GetGeometryPtr() {
			return mdl_vertices;
		}

		Uint32 Model::VertexCount() {
			return vertices;
		}

		// Rigged model

		RiggedModel::RiggedModel(RiggedGeometry* geom, Skeleton* skeleton) {
			mdl_vertices = (Vertex*)GetGeomAllocator()->Allocate(sizeof(Vertex) * geom->vertex_count);
			memcpy(mdl_vertices, geom->vertices, sizeof(Vertex) * geom->vertex_count);
			vertices = geom->vertex_count;

			this->skeleton = skeleton;
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, geom->vertex_count * vertex_size, geom->vertices, GL_STATIC_DRAW);
			glGenBuffers(1, &w_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, w_vbo);
			glBufferData(GL_ARRAY_BUFFER, geom->vertex_count * weight_size, geom->weights, GL_STATIC_DRAW);
			Uint32 offset = 0;
			for (Uint32 i = 0; i < geom->meshes; ++i) {
				meshes.push_back(new RiggedMesh(geom->materials[i], vbo, w_vbo, geom->indices_count[i], &geom->indices[offset]));
				offset += geom->indices_count[i];
			}
		}

		RiggedModel::~RiggedModel() {
			GetGeomAllocator()->Deallocate(mdl_vertices);
			glDeleteBuffers(1, &vbo);
			for(Uint32 i = 0; i < meshes.size(); i++) {
				delete meshes[i];
			}
		}

		void RiggedModel::Draw(Shader* shader) {
			for (Uint32 i = 0; i < meshes.size(); ++i) {
				meshes[i]->Draw(shader);
			}
		}

		std::vector<RiggedMesh*>& RiggedModel::GetMeshes() {
			return meshes;
		}

		GLuint RiggedModel::GetVBO() {
			return vbo;
		}

		Skeleton* RiggedModel::GetSkeleton() {
			return skeleton;
		}

		Vertex* RiggedModel::GetGeometryPtr() {
			return mdl_vertices;
		}

		Uint32 RiggedModel::VertexCount() {
			return vertices;
		}

	}
}
