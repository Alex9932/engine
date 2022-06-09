/*
 * window.h
 *
 *  Created on: Feb 11, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_WINDOW_H_
#define ENGINE_RENDER_WINDOW_H_

#include <engine/core/math/vector.h>
#include <engine/engine.h>
#include <SDL2/SDL.h>

namespace Engine {
	namespace Window {

		RG_DECLSPEC void Initialize();
		RG_DECLSPEC void Destroy();
		RG_DECLSPEC void Show();
		RG_DECLSPEC void Update();
		RG_DECLSPEC void ToggleFullscreen();
		RG_DECLSPEC bool IsFullscreen();
		RG_DECLSPEC SDL_Window* GetWindow();
		RG_DECLSPEC void GetWindowSize(Engine::Math::vec2* size);
		RG_DECLSPEC void SetFpsLimit(Sint32 fps);

	}
}


#endif /* ENGINE_RENDER_WINDOW_H_ */
