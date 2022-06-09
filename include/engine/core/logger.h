/*
 * logger.h
 *
 *  Created on: Feb 10, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_LOGGER_H_
#define ENGINE_LOGGER_H_

#include <SDL2/SDL.h>
#include <engine/engine.h>

#define RG_LOG_GAME   SDL_LOG_CATEGORY_APPLICATION
#define RG_LOG_SYSTEM SDL_LOG_CATEGORY_SYSTEM
#define RG_LOG_RENDER SDL_LOG_CATEGORY_RENDER
#define RG_LOG_ERROR  SDL_LOG_CATEGORY_ERROR
#define RG_LOG_ASSERT SDL_LOG_CATEGORY_ASSERT
#define RG_LOG_AUDIO  SDL_LOG_CATEGORY_AUDIO
#define RG_LOG_DEBUG  SDL_LOG_CATEGORY_CUSTOM

#define rgLogInfo     SDL_LogInfo
#define rgLogWarn     SDL_LogWarn
#define rgLogError    SDL_LogError
#define rgLogCritical SDL_LogCritical
#define rgLogDebug    SDL_LogDebug

namespace Engine {
	namespace Logger {
		RG_DECLSPEC void Initialize();
		RG_DECLSPEC void Destroy();
		RG_DECLSPEC const char* GetLine(Uint32 i);
		RG_DECLSPEC SDL_LogPriority GetLinePriority(Uint32 i);
		RG_DECLSPEC Uint32 GetLines();
		RG_DECLSPEC Uint32 GetMessages();
	}
}

#endif /* ENGINE_LOGGER_H_ */
