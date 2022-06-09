/*
 * event.cpp
 *
 *  Created on: Feb 11, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/core/event.h>
#include <vector>

#include <imgui_impl_sdl.h>

namespace Engine {
	namespace Event {
		static std::vector<EventHandler> handlers;
		static SDL_Event event;

		static void UpdateHandlers(SDL_Event* event) {
			for (Uint32 i = 0; i < handlers.size(); ++i) {
				handlers[i](event);
			}
		}

		void PushEvent(Uint32 w_id, Sint32 code, void* ptr1, void* ptr2) {
			SDL_Event q_event;
			q_event.type = SDL_USEREVENT;
			q_event.user.timestamp = SDL_GetTicks();
			q_event.user.windowID = w_id;
			q_event.user.code = code;
			q_event.user.data1 = ptr1;
			q_event.user.data2 = ptr2;
			UpdateHandlers(&q_event);
		}

		void Initialize() {
			PushEvent(0xFFFFFFF, RG_EVENT_START, NULL, NULL);
		}

		void Destroy() {
			PushEvent(0xFFFFFFF, RG_EVENT_STOP, NULL, NULL);
			handlers.clear();
		}

		void HandleEvents() {
			while(SDL_PollEvent(&event)) {

				if(GetGame()->isClient()) {
					ImGui_ImplSDL2_ProcessEvent(&event);
				}

				UpdateHandlers(&event);
			}
		}

		void RegisterEventHandler(EventHandler handler) {
			handlers.push_back(handler);
			PushEvent(0xFFFFFFF, RG_EVENT_NEW_HANDLER, (void*)handler, NULL); // Handler function's pointer in first argument
		}

	}
}
