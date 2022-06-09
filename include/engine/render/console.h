/*
 * console.h
 *
 *  Created on: Apr 3, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_CONSOLE_H_
#define ENGINE_RENDER_CONSOLE_H_

#include <engine/engine.h>

namespace Engine {
	namespace Render {

		RG_DECLSPEC void InitConsole();
		RG_DECLSPEC void RenderConsole();
		RG_DECLSPEC void DestroyConsole();
		RG_DECLSPEC void SetConsoleShown(bool s);

	}
}


#endif /* ENGINE_RENDER_CONSOLE_H_ */
