/*
 * mesh.h
 *
 *  Created on: Feb 28, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_CORE_MESH_H_
#define ENGINE_CORE_MESH_H_

#include <engine/core/math/vector.h>
#include <engine/render/material.h>
#include <engine/render/shader.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>

namespace Engine {
	namespace Render {

		typedef struct Vertex {
			Math::vec3 vertex;
			Math::vec3 normal;
			Math::vec3 tangent;
			Math::vec2 t_coord;
		} Vertex;

		typedef struct Weight {
			Math::vec4 weights;
			Uint32 id[4];
		} Weight;

		class RG_DECLSPEC Mesh {
			private:
				GLuint vao;
				GLuint ebo;
				Uint32 count;
				Material* material;

			public:
				Mesh(Material* material, GLuint vbo, Uint32 count, Uint32* indices);
				virtual ~Mesh();
				void Draw(SShader* shader);
				Material* GetMaterial();
		};

		class RG_DECLSPEC RiggedMesh {
			private:
				GLuint vao;
				GLuint ebo;
				Uint32 count;
				Material* material;

			public:
				RiggedMesh(Material* material, GLuint vbo, GLuint w_vbo, Uint32 count, Uint32* indices);
				virtual ~RiggedMesh();
				void Draw(SShader* shader);
				Material* GetMaterial();
		};

	}
}

#endif /* ENGINE_CORE_MESH_H_ */
