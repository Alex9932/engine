/*
 * profiler.cpp
 *
 *  Created on: Feb 21, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/core/profiler.h>

namespace Engine {

	Profiler::Profiler() {
		this->timer = new Timer();
//		this->frametime = 0;
		this->current = NULL;
	}

	Profiler::~Profiler() {
		delete this->timer;
	}

	void Profiler::Reset() {
//		this->frametime = this->timer->GetElapsedTime();
		this->timer->Update();
	}

	void Profiler::Begin(String s) {
		if(current != NULL) {
			map[current] = this->timer->GetElapsedTime();
			this->timer->Update();
		}
		current = s;
	}

	double Profiler::GetValue(String s) {
		return map[s];
	}

//	double Profiler::GetFramerate() {
//		if(this->frametime == 0) {
//			this->frametime = 0.001;
//		}
//		return 1.0 / this->frametime;
//	}
//
//	double Profiler::GetFrametime() {
//		return this->frametime;
//	}

}
