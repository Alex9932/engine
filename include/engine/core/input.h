/*
 * input.h
 *
 *  Created on: Feb 13, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_CORE_INPUT_H_
#define ENGINE_CORE_INPUT_H_

#include <engine/engine.h>
#include <SDL2/SDL.h>

namespace Engine {
	namespace Input {

		RG_DECLSPEC bool IsKeyDown(SDL_Scancode scancode);
		RG_DECLSPEC bool IsButtonDown(Uint8 btn);
		RG_DECLSPEC void SetMouseSensitivity(double sens);
		RG_DECLSPEC double GetMouseSensitivity();
		RG_DECLSPEC double GetMouseX();
		RG_DECLSPEC double GetMouseY();
		RG_DECLSPEC double GetMouseDX();
		RG_DECLSPEC double GetMouseDY();
		RG_DECLSPEC double GetMouseDW(); // Mouse wheel

		RG_DECLSPEC void Initialize();
		RG_DECLSPEC void Destroy();
		RG_DECLSPEC void Update();

	}
}


#endif /* ENGINE_CORE_INPUT_H_ */
