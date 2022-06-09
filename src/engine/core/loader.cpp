/*
 * loader.cpp
 *
 *  Created on: Feb 23, 2022
 *      Author: alex9932
 */

#define STB_IMAGE_IMPLEMENTATION
#define DLL_EXPORT

#include <engine/core/loader.h>
#include <engine/core/strings.h>
#include <engine/core/filesystem.h>
#include <stb/stb_image.h>
#include <GL/glew.h>

#include <vector>

namespace Engine {
	namespace Loader {

		typedef struct TEXTURE_STRING {
			GLuint id;
			char path[128];
		} TEXTURE_STRING;

		static std::vector<TEXTURE_STRING> textures;
		static bool ns_anisotropic = false;

//		RG_FORCE_INLINE static bool TEX_CMP(const TEXTURE_STRING& ts, String s) {
//			if(Helper::streql(ts.path, s)) { return true; }
//			return false;
//		}

		static GLuint FindLoadedTexture(String s) {
			for (size_t i = 0; i < textures.size(); ++i) {
//				if(TEX_CMP(textures[i], s)) {
				if(Helper::streql(textures[i].path, s)) {
					return textures[i].id;
				}
			}

			return 0;
		}

		GLuint LoadTexture(String path) {
			GLuint t = FindLoadedTexture(path);
			if(t != 0) { return t; }

			TEXTURE_STRING ts;
			size_t len = SDL_min(SDL_strlen(path), 128);
			SDL_memset(ts.path, 0, 128);
			SDL_memcpy(ts.path, path, len);

			GLuint texture;
			SDL_LogInfo(SDL_LOG_CATEGORY_RENDER, "Load texture: %s", path);

			int channels = 4;
			int width = 0;
			int height = 0;
			unsigned char* data = NULL;

			Engine::Filesystem::Resource* res = Engine::Filesystem::GetResource(path);

			data = stbi_load_from_memory((stbi_uc const*)res->data, res->length, &width, &height, &channels, 0);
			RG_ASSERT_MSG(data, "TEXTURE ERROR");

			GLenum format = GL_RGBA;
			GLenum ftype = GL_RGBA;
			if (channels == 3) {
				format = GL_RGB;
				ftype = GL_RGB;
			}

			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, ftype, GL_UNSIGNED_BYTE, data);

			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1);

			if (GLEW_EXT_texture_filter_anisotropic) {
				GLfloat max = 0;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max);
				glTexParameterf(GL_TEXTURE_2D, GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, SDL_max(4.0, max));
			} else {
				if(!ns_anisotropic) {
					SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "OpenGL: Anisotropic filtering not supported!");
					ns_anisotropic = true;
				}
			}

			stbi_image_free(data);

			Engine::Filesystem::FreeResource(res);

			ts.id = texture;
			textures.push_back(ts);

			return texture;
		}

		void FreeTexture(GLuint t) {
			glDeleteTextures(1, &t);
		}

		Sound::SoundData LoadSound(Sound::SoundLoader* loader, String path) {
			return loader->LoadSound(path);
		}

		void FreeSound(Sound::SoundData s) {
			free(s.data);
		}

		Geometry* LoadGeometry(GeometryLoader* loader, String path){
			return loader->LoadGeometry(path);
		}

		void FreeGeometry(Geometry* geom) {
			free(geom->indices_count);
			free(geom->materials);
			free(geom->vertices);
			free(geom->indices);
			free(geom);
		}

	}
}
