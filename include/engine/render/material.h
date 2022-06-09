/*
 * material.h
 *
 *  Created on: Feb 28, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_MATERIAL_H_
#define ENGINE_RENDER_MATERIAL_H_

#include <GL/glew.h>
#include <engine/engine.h>
#include <engine/core/math/vector.h>

namespace Engine {
	namespace Render {

		class RG_DECLSPEC Material {
			private:
				char name[64];
				char name2[64]; // Additional texture (loaded as normal map)
				bool loaded = false;
				GLuint diffuse = 0;
				GLuint normal = 0;
				Engine::Math::vec4 d_color = {1, 1, 1, 1};
				Engine::Math::vec3 a_color = {1, 1, 1};
				Engine::Math::vec3 s_color = {1, 1, 1};
				float shininess = 0;

			public:
				Material(String material);
				Material(String tex0, String tex1);
				virtual ~Material();
				void Load();
				void Bind();
				bool IsLoaded();

				void SetColor(const Engine::Math::vec4& color);
				Engine::Math::vec4& GetColor();
				void SetAmbient(const Engine::Math::vec3& color);
				Engine::Math::vec3& GetAmbient();
				void SetSpecular(const Engine::Math::vec3& color);
				Engine::Math::vec3& GetSpecular();
				void SetShininess(float shininess);
				float GetShininess();
		};

	}
}

#endif /* ENGINE_RENDER_MATERIAL_H_ */
