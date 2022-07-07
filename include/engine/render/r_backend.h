/*
 * r_backend.h
 *
 *  Created on: Jul 7, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_R_BACKEND_H_
#define ENGINE_RENDER_R_BACKEND_H_

#include <engine/engine.h>

#define R_OPENGL_RENDERER

namespace Engine {
	namespace Render {

		RG_DECLSPEC void SetupBackend();
		RG_DECLSPEC void InitializeBackend();
		RG_DECLSPEC void DestroyBackend();
		RG_DECLSPEC void Swapbuffers();
		RG_DECLSPEC void ViewportBackend(float x, float y, float w, float h);
		RG_DECLSPEC void MakeImguiBackend();

		RG_DECLSPEC String GetRendererName();

	}
}


#endif /* ENGINE_RENDER_R_BACKEND_H_ */
