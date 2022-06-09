/*
 * timer.cpp
 *
 *  Created on: Feb 13, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/core/timer.h>
#include <engine/core/logger.h>

namespace Engine {
	Timer::Timer() {
		this->m_time = SDL_GetPerformanceCounter();
//		this->delta = 0;
		this->time = 0;
	}

	Timer::~Timer() {}

	double Timer::GetElapsedTime() {
		return (double)(SDL_GetPerformanceCounter() - this->m_time) / (double)SDL_GetPerformanceFrequency();
	}

	double Timer::GetTime() {
		return time;
	}

	void Timer::Update() {
		double delta = (double)(SDL_GetPerformanceCounter() - this->m_time) / (double)SDL_GetPerformanceFrequency();
		m_time = SDL_GetPerformanceCounter();
		time += delta;
//		rgLogInfo(RG_LOG_SYSTEM, "Time: %lf (%lf)", time, delta);
	}
}
