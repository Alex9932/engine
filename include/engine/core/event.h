/*
 * event.h
 *
 *  Created on: Feb 11, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_EVENT_H_
#define ENGINE_EVENT_H_

#include <engine/engine.h>
#include <SDL2/SDL.h>

#define RG_EVENT_STOP        0x0000001
#define RG_EVENT_START       0x0000002
#define RG_EVENT_NEW_HANDLER 0x0000004
#define RG_EVENT_COMMAND     0x0000008

typedef bool (*EventHandler)(SDL_Event*);

namespace Engine {
	namespace Event {

		RG_DECLSPEC void Initialize();
		RG_DECLSPEC void Destroy();

		RG_DECLSPEC void HandleEvents();
		RG_DECLSPEC void RegisterEventHandler(EventHandler handler);
		RG_DECLSPEC void PushEvent(Uint32 w_id, Sint32 code, void* ptr1, void* ptr2);

	}
}


#endif /* ENGINE_EVENT_H_ */
