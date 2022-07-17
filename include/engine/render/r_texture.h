/*
 * r_texture.h
 *
 *  Created on: Jul 17, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_R_TEXTURE_H_
#define ENGINE_RENDER_R_TEXTURE_H_


#include <engine/engine.h>

namespace Engine {
	namespace Render {

		enum TextureDataType {
			DATATYPE_FLOAT = 0,
			DATATYPE_UINT8
		};

		enum TextureFormat {
			FORMAT_R = 0,
			FORMAT_RG,
			FORMAT_RGB,
			FORMAT_RGBA
		};

		enum TextureFilter {
			FILTER_NEAREST = 0,
			FILTER_LINEAR
		};

		typedef struct TextureInfo {
			Uint32 width;
			Uint32 height;
			TextureDataType type;
			TextureFormat format;
			TextureFilter filter;
			void* data;
		} TextureInfo;

		class RG_DECLSPEC Texture {

			protected:
				Uint32 width  = 0;
				Uint32 height = 0;

			public:
				Texture(const TextureInfo& info) {}
				virtual ~Texture() {}

				virtual void SubData(Uint32 xoffset, Uint32 yoffset, Uint32 width, Uint32 height, void* data) {}

		};

	}
}


#endif /* ENGINE_RENDER_R_TEXTURE_H_ */
