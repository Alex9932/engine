/*
 * render2d.h
 *
 *  Created on: Feb 22, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_RENDER2D_H_
#define ENGINE_RENDER_RENDER2D_H_


#include <engine/core/math/matrix.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <engine/render/r_texture.h>

namespace Engine {
	namespace Render {

		typedef struct R2D_VERTEX {
			Engine::Math::vec2 pos;
			Engine::Math::vec2 coord;
			Engine::Math::vec4 color;
		} R2D_VERTEX;

		RG_DECLSPEC void InitializeR2D();
		RG_DECLSPEC void DestroyR2D();

		RG_DECLSPEC void R2D_BindTexture(Texture* tex);
		RG_DECLSPEC void R2D_SetColor(const Engine::Math::vec4& color);
		RG_DECLSPEC void R2D_SetMatrix(const Engine::Math::mat4& mat);
		RG_DECLSPEC void R2D_PushVertex(const R2D_VERTEX& vtx);
		RG_DECLSPEC void R2D_DrawMesh();
		RG_DECLSPEC void R2D_DrawLines();
		RG_DECLSPEC void R2D_DrawQuad(const Engine::Math::vec2& pos, const Engine::Math::vec2& size);
		RG_DECLSPEC void R2D_DrawCircle(const Engine::Math::vec2& pos, float radius, Uint32 n);

	}
}


#endif /* ENGINE_RENDER_RENDER2D_H_ */
