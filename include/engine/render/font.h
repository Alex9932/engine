/*
 * font.h
 *
 *  Created on: Feb 17, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_FONT_H_
#define ENGINE_RENDER_FONT_H_

#include <engine/engine.h>
#include <GL/glew.h>


#define F_ATLAS_WIDTH  32
#define F_ATLAS_HEIGHT 64

namespace Engine {
	namespace Render {

		typedef struct Glyph {
			int size_x;
			int size_y;
			int bearing_x;
			int bearing_y;
			Uint32 advance_x;
			Uint32 advance_y;
		} Glyph;

		class RG_DECLSPEC Font {
			private:
				GLuint font_atlas;
				Uint32 scale;
				Glyph chars[F_ATLAS_WIDTH * F_ATLAS_HEIGHT];

			public:
				Font(String file, Uint32 scale);
				virtual ~Font();
				GLuint GetAtlas();
				Uint32 GetScale();
				Glyph* GetGlyphs();
				float GetRawStringLength(const Uint16* str, float scale);
				float GetStringLength(String str, float scale);
				float GetRawStringLength(const Uint16* str, float scale, Uint32 len);
				float GetStringLength(String str, float scale, Uint32 len);

		};

	}
}

#endif /* ENGINE_RENDER_FONT_H_ */
