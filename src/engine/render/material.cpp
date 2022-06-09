/*
 * material.cpp
 *
 *  Created on: Feb 28, 2022
 *      Author: alex9932
 */

#define _CRT_SECURE_NO_WARNINGS
#define DLL_EXPORT

#include <engine/render/material.h>
#include <engine/core/loader.h>

#include <cstdio>

namespace Engine {
	namespace Render {

		static String mat_path = "gamedata/materials/";
		static String diffuse_map = "diffuse.png";
		static String normal_map = "normal.png";

		Material::Material(String material) {
			sprintf(name, "%s", material);
			name2[0] = 0;
		}

		Material::Material(String tex0, String tex1) {
			sprintf(name, "%s", tex0);
			sprintf(name2, "%s", tex1);
		}

		Material::~Material() {
			Loader::FreeTexture(diffuse);
			Loader::FreeTexture(normal);
		}

		void Material::Load() {
			if(name[0] == '&') {
				diffuse = Loader::LoadTexture(&name[1]);
				normal = 0;
				if(name2[0] == '&') {
					normal = Loader::LoadTexture(&name2[1]);
				}
			} else {
				char d_path[128];
				char n_path[128];
				sprintf(d_path, "%s%s/%s", mat_path, name, diffuse_map);
				sprintf(n_path, "%s%s/%s", mat_path, name, normal_map);
				diffuse = Loader::LoadTexture(d_path);
				normal = Loader::LoadTexture(n_path);
			}
			loaded = true;
		}

		void Material::Bind() {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuse);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, normal);
		}

		bool Material::IsLoaded() {
			return loaded;
		}

		void Material::SetColor(const Engine::Math::vec4& color) {
			this->d_color = color;
		}

		void Material::SetAmbient(const Engine::Math::vec3& color) {
			this->a_color = color;
		}

		void Material::SetSpecular(const Engine::Math::vec3& color) {
			this->s_color = color;
		}

		Engine::Math::vec4& Material::GetColor() {
			return d_color;
		}

		Engine::Math::vec3& Material::GetAmbient() {
			return a_color;
		}

		Engine::Math::vec3& Material::GetSpecular() {
			return s_color;
		}

		void Material::SetShininess(float shininess) {
			this->shininess = shininess;
		}

		float Material::GetShininess() {
			return shininess;
		}

	}
}
