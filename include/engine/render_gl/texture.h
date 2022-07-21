/*
 * texture.h
 *
 *  Created on: Jul 17, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_GL_TEXTURE_H_
#define ENGINE_RENDER_GL_TEXTURE_H_

#include <engine/render/r_texture.h>
#include <GL/glew.h>

namespace Engine {
	namespace Render {

		class RG_DECLSPEC GLTexture : public Texture {

			protected:
				GLenum format;
				GLenum type;
				GLuint texture;

			public:
				GLTexture(const TextureInfo& info);
				virtual ~GLTexture();

				virtual void SubData(Uint32 xoffset, Uint32 yoffset, Uint32 width, Uint32 height, void* data);

				RG_INLINE GLuint GetGLTextureID() { return texture; }

		};

	}
}


#endif /* ENGINE_RENDER_GL_TEXTURE_H_ */
