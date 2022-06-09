/*
 * profiler.h
 *
 *  Created on: Feb 21, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_CORE_PROFILER_H_
#define ENGINE_CORE_PROFILER_H_


#include <engine/core/timer.h>
#include <engine/engine.h>

#include <map>

namespace Engine {

	class RG_DECLSPEC Profiler {
		private:
			Timer* timer;
			std::map<String, double> map;
			String current;
//			double frametime;

		public:
			Profiler();
			virtual ~Profiler();

			void Reset();

			void Begin(String s);
			double GetValue(String s);

//			double GetFramerate();
//			double GetFrametime();
	};

}


#endif /* ENGINE_CORE_PROFILER_H_ */
