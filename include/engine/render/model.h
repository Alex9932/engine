/*
 * model.h
 *
 *  Created on: Feb 28, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_MODEL_H_
#define ENGINE_RENDER_MODEL_H_

#include <vector>
#include <GL/glew.h>
#include <engine/render/mesh.h>
#include <engine/render/shader.h>
#include <engine/core/geometry.h>
#include <engine/core/skeleton.h>

namespace Engine {
	namespace Render {

		class RG_DECLSPEC Model {
			private:
				GLuint vbo; // Vertices data
				std::vector<Mesh*> meshes; // Indices, materials, etc
				Vertex* mdl_vertices; // Full models's geometry
				Uint32 vertices;

			public:
				Model(Geometry* geom);
				virtual ~Model();
				void Draw(Shader* shader);
				std::vector<Mesh*>& GetMeshes();
				GLuint GetVBO();
				Engine::Render::Vertex* GetGeometryPtr();
				Uint32 VertexCount();
		};

		class RG_DECLSPEC RiggedModel {
			private:
				GLuint vbo; // Vertices data
				GLuint w_vbo; // Weights data
				std::vector<RiggedMesh*> meshes; // Indices, materials, etc
				Vertex* mdl_vertices; // Full models's geometry
				Skeleton* skeleton;
				Uint32 vertices;

			public:
				RiggedModel(RiggedGeometry* geom, Skeleton* skeleton);
				virtual ~RiggedModel();
				void Draw(Shader* shader);
				std::vector<RiggedMesh*>& GetMeshes();
				GLuint GetVBO();
				Skeleton* GetSkeleton();
				Engine::Render::Vertex* GetGeometryPtr();
				Uint32 VertexCount();
		};

	}
}

#endif /* ENGINE_RENDER_MODEL_H_ */
