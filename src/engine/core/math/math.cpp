/*
 * math.cpp
 *
 *  Created on: Feb 10, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/core/math/math.h>
#include <SDL2/SDL.h>

namespace Engine {
	namespace Math {

		const mat4 IDENTITY_MAT3 = {
				1, 0, 0,
				0, 1, 0,
				0, 0, 1};

		const mat4 IDENTITY_MAT4 = {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1};

		double toradians(double angle) {
			return (angle * (RG_PI / 180.0));
		}

		double todegrees(double angle) {
			return (angle * (180.0 / RG_PI));
		}

		double invsqrt(double a) {
			return 1.0 / SDL_sqrt(a);
		}

	}
}
