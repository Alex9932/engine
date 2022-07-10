/*
 * engine.h
 *
 *	Source code based on rgEngine v0.6
 *
 *  Created on: Feb 9, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#define RG_INLINE inline
#if _WIN32 || _WIN64
	#define RG_FORCE_INLINE inline
	#define RG_PLATFORM_NAME "WIN32"
	#define RG_PLATFORM_WINDOWS
	#include <windows.h>
	#ifdef DLL_EXPORT
		#define RG_DECLSPEC __declspec(dllexport)
	#else
		#define RG_DECLSPEC __declspec(dllimport)
	#endif
	#if _WIN64
		#define RG_ENV64
	#else
		#define RG_ENV32
	#endif
#elif __GNUC__
	#define RG_FORCE_INLINE inline __attribute__((always_inline))
	#define RG_PLATFORM_NAME "LINUX"
	#define RG_PLATFORM_LINUX
	#define RG_DECLSPEC
	#if __x86_64__ || __ppc64__
		#define RG_ENV64
	#else
		#define RG_ENV32
	#endif
#else
	#define RG_FORCE_INLINE inline
	#define RG_PLATFORM_NAME "UNKNOWN"
	#define RG_DECLSPEC
	#define RG_ENV32
	#error "INVALID ENVIRONMENT"
#endif

#ifdef RG_ENV64
#define RG_CASTADDR(a) ((void*)(Uint64)a)
#else
#define RG_CASTADDR(a) ((void*)a)
#endif


typedef const char* String;
typedef const wchar_t* WString;

#include <SDL2/SDL.h>
#include <engine/core/logger.h>

#define RG_ERROR				Engine::HandleError("");
#define RG_ERROR_MSG(msg)       Engine::HandleError(msg);
#define RG_ASSERT               SDL_assert_always
#define RG_ASSERT_MSG(c, msg)   if(c == 0) rgLogError(RG_LOG_ASSERT, "Internal error! %s", msg); \
								if(c == 0) Engine::rg_assert_message = msg;                      \
								SDL_assert_always(c)

// TODO !!! Remove this !!!
extern RG_DECLSPEC String RG_PROFILER_SEGMENT_wait_jobs;
extern RG_DECLSPEC String RG_PROFILER_SEGMENT_main_update;
extern RG_DECLSPEC String RG_PROFILER_SEGMENT_sound;
extern RG_DECLSPEC String RG_PROFILER_SEGMENT_window_update;
extern RG_DECLSPEC String RG_PROFILER_SEGMENT_process_events;
extern RG_DECLSPEC String RG_PROFILER_SEGMENT_other;

namespace Engine {

	class Profiler;
	class UTF8Decoder;
	class UTF8Encoder;

	extern RG_DECLSPEC String rg_assert_message;

	class RG_DECLSPEC BaseGame {
		protected:
			bool is_client = false;

		public:
			BaseGame() {}
			virtual ~BaseGame() {}
			virtual void MainUpdate() {}
			virtual void Initialize() {}
			virtual void Quit() {}
			bool isClient();
	};

	RG_DECLSPEC void Initialize(BaseGame& game);
	RG_DECLSPEC void Start();
	RG_DECLSPEC void Quit();
	RG_DECLSPEC void ForceQuit();
	RG_DECLSPEC void Error(String error);

	RG_DECLSPEC int ProcessArguments(int argc, String* argv);

	RG_DECLSPEC void HandleError(String str);

	RG_DECLSPEC bool IsArch64();
	RG_DECLSPEC bool IsDebug();
	RG_DECLSPEC Uint32 GetThreads();
	RG_DECLSPEC double GetDeltaTime();
	RG_DECLSPEC double GetUptime();
	RG_DECLSPEC BaseGame* GetGame();
	RG_DECLSPEC Profiler* GetProfiler();

	RG_DECLSPEC int GetExitCode();

	RG_DECLSPEC String GetEngineVersion();
	RG_DECLSPEC String GetEnginePlatform();
	RG_DECLSPEC String GetGLInfo();
	RG_DECLSPEC String GetUsername();

	RG_DECLSPEC UTF8Decoder* GetUTF8Decoder();
	RG_DECLSPEC UTF8Encoder* GetUTF8Encoder();
}

#endif /* ENGINE_H_ */
