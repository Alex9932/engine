/*
 * timer.h
 *
 *  Created on: Feb 13, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_CORE_TIMER_H_
#define ENGINE_CORE_TIMER_H_

#include <SDL2/SDL.h>
#include <engine/engine.h>

namespace Engine {
	class RG_DECLSPEC Timer {
		private:
			Uint64 m_time;
//			double delta;
			double time;

		public:
			Timer();
			~Timer();

			double GetElapsedTime(); // In seconds
			double GetTime(); // In seconds
			void Update();
	};
}


#endif /* ENGINE_CORE_TIMER_H_ */
