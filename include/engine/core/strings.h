/*
 * strings.h
 *
 *  Created on: Feb 10, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_STRINGS_H_
#define ENGINE_STRINGS_H_

#include <engine/engine.h>
#include <SDL2/SDL.h>

namespace Engine {
	namespace Helper {

		RG_DECLSPEC String getCategory(int category);
		RG_DECLSPEC String getPriority(SDL_LogPriority priority);
		RG_DECLSPEC String getCpuName(int cores);

		RG_DECLSPEC bool streql(String left, String right);
		RG_DECLSPEC bool strstw(String left, String right);

	}
}


#endif /* ENGINE_STRINGS_H_ */
