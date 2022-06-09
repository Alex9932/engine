/*
 * fontrenderer.h
 *
 *  Created on: Feb 21, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_FONTRENDERER_H_
#define ENGINE_RENDER_FONTRENDERER_H_

#include <engine/engine.h>
#include <engine/render/font.h>
#include <engine/core/math/vector.h>


namespace Engine {
	namespace Render {

		RG_DECLSPEC void InitFontRenderer();
		RG_DECLSPEC void DestroyFontRenderer();
		RG_DECLSPEC void RenderRawString(Font* font, Uint16* str, const Engine::Math::vec2& pos, const Engine::Math::vec4& color, float scale);
		RG_DECLSPEC void RenderString(Font* font, String str, const Engine::Math::vec2& pos, const Engine::Math::vec4& color, float scale);

	}
}


#endif /* ENGINE_RENDER_FONTRENDERER_H_ */
