/*
 * input.cpp
 *
 *  Created on: Feb 13, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/core/input.h>
#include <engine/core/event.h>


namespace Engine {
	namespace Input {

		static bool keys[1024];
		static bool m_btns[64];

		static double m_x  = 0.0;
		static double m_y  = 0.0;
		static double m_dx = 0.0;
		static double m_dy = 0.0;
		static double m_dw = 0.0;
		static double m_tdx = 0.0;
		static double m_tdy = 0.0;
		static double m_tdw = 0.0;
		static double m_sens = 0.07;

		bool IsKeyDown(SDL_Scancode scancode) {
			return keys[scancode];
		}

		bool IsButtonDown(Uint8 btn) {
			return m_btns[btn];
		}

		double GetMouseX() {
			return m_x;
		}

		double GetMouseY() {
			return m_y;
		}

		double GetMouseDX() {
			return m_dx;
		}

		double GetMouseDY() {
			return m_dy;
		}

		double GetMouseDW() {
			return m_dw;
		}

		double GetMouseSensitivity() {
			return m_sens;
		}

		void SetMouseSensitivity(double sens) {
			m_sens = sens;
		}

		bool InputHandler(SDL_Event* event) {

			if(event->type == SDL_MOUSEMOTION) {
				m_tdx += ((double)event->motion.x - m_x) * m_sens;
				m_tdy += ((double)event->motion.y - m_y) * m_sens;
				m_x = (double)event->motion.x;
				m_y = (double)event->motion.y;
			}

			if(event->type == SDL_MOUSEWHEEL) {
				m_tdw += event->wheel.y;
			}

			if(event->type == SDL_KEYDOWN || event->type == SDL_KEYUP) {
				keys[event->key.keysym.scancode] = (event->type == SDL_KEYUP ? false : true);
			}

			if(event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP) {
				m_btns[event->button.button] = (event->type == SDL_MOUSEBUTTONUP ? false : true);
			}

//			switch (event->type) {
//				case SDL_KEYDOWN:
//
//					break;
//				case SDL_KEYUP:
//
//					break;
//				case SDL_MOUSEBUTTONDOWN:
//
//					break;
//				case SDL_MOUSEBUTTONUP:
//
//					break;
//				case SDL_MOUSEWHEEL:
//
//					break;
//				case SDL_MOUSEMOTION:
//
//					break;
//				default:
//					break;
//			}

			return true;
		}

		void Initialize() {
			Engine::Event::RegisterEventHandler(InputHandler);
			SDL_memset(keys, 0, sizeof(keys));
			SDL_memset(m_btns, 0, sizeof(m_btns));
		}

		void Destroy() {

		}

		void Update() {
			m_dx = m_tdx;
			m_dy = m_tdy;
			m_dw = m_tdw;
			m_tdx = 0;
			m_tdy = 0;
			m_tdw = 0;
		}

	}
}
