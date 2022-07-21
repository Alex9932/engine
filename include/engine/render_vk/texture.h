/*
 * texture.h
 *
 *  Created on: Jul 17, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_VK_TEXTURE_H_
#define ENGINE_RENDER_VK_TEXTURE_H_

#include <engine/render/r_texture.h>

namespace Engine {
	namespace Render {

		class RG_DECLSPEC VKTexture : public Texture {

			protected:

			public:
				VKTexture(const TextureInfo& info);
				virtual ~VKTexture();

				virtual void SubData(Uint32 xoffset, Uint32 yoffset, Uint32 width, Uint32 height, void* data);

		};

	}
}


#endif /* ENGINE_RENDER_VK_TEXTURE_H_ */
