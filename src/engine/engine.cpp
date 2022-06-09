/*
 * engine.cpp
 *
 *  Created on: Feb 9, 2022
 *      Author: alex9932
 */

#define _CRT_SECURE_NO_WARNINGS
#define DLL_EXPORT

#include <engine/engine.h>
#include <engine/core/strings.h>
#include <engine/core/event.h>
#include <engine/core/input.h>
#include <engine/core/timer.h>
#include <engine/core/utf8.h>
#include <engine/core/profiler.h>
#include <engine/core/filesystem.h>
#include <engine/core/thread/thread.h>
#include <engine/core/command/command.h>
#include <engine/render/window.h>
#include <engine/render/render.h>
#include <engine/sound/sound.h>

#ifdef RG_PLATFORM_WINDOWS
#include <Lmcons.h>
#else

#endif

#define RG_BUILD         51
#define RG_VERSION_MAJ   0
#define RG_VERSION_MIN   1
#define RG_VERSION_PATCH 28

static const Uint32 RG_SDL_INIT_CLIENT_FLAGS =
	SDL_INIT_AUDIO |
	SDL_INIT_VIDEO |
	SDL_INIT_JOYSTICK |
	SDL_INIT_HAPTIC |
	SDL_INIT_GAMECONTROLLER |
	SDL_INIT_SENSOR;

String RG_PROFILER_SEGMENT_wait_jobs      = "wait_jobs";
String RG_PROFILER_SEGMENT_main_update    = "main_update";
String RG_PROFILER_SEGMENT_sound          = "sound";
String RG_PROFILER_SEGMENT_window_update  = "window_update";
String RG_PROFILER_SEGMENT_process_events = "process_events";
String RG_PROFILER_SEGMENT_other          = "other";

// TODO

/*
 * fstab:
 *  {
 *   "platform": "./platform"
 *   "gamedata": "./gamedata"
 *
 *   "database": [
 *    "pkg0.rfs",
 *    "pkg1.rfs"
 *    ...
 *   ]
 *  }
 *
 *
 * .mat
 *  [header]
 *   "RMT " - 4-byte magic
 *
 *   // textures /gamedata/textures/[texture]
 *   color texture - 128-byte null-terminated string
 *   normal texture - 128-byte null-terminated string
 *
 *   gamedata/materials/?.mat
 *   gamedata/textures/?.?
 */


namespace Engine {
	String rg_assert_message;

	static char version_str[32];
	static char platform_str[32];
	static char opengl_str[256];
	static char user_profile_str[64];

	static bool is_arch64 = false;
	static bool is_debug = false;
	static bool running  = true;
	static Uint32 num_threads = 8;
	static BaseGame* game_ptr;
	static Timer timer;
	static double uptime = 0;
	static double frame_time;

	static Profiler* profiler;
	static UTF8Decoder utf8_decoder;
	static UTF8Encoder utf8_encoder;

	static int exit_code = 0;

	static bool MainHandler(SDL_Event* event) {

		if(event->type == SDL_QUIT) {
			Quit();
		}

		return true;
	}

	static SDL_AssertState AssertionHandler(const SDL_AssertData* data, void* userdata) {
		rgLogError(RG_LOG_ASSERT, "Assertion failure at %s (%s:%d) '%s' %s", data->function, data->filename, data->linenum, data->condition, rg_assert_message);

		if(game_ptr->isClient()) {
			char message[2048];
			sprintf(message, "Assertion failure at %s (%s:%d) '%s'\n%s", data->function, data->filename, data->linenum, data->condition, rg_assert_message);
			Error(message);
		}

		return SDL_ASSERTION_BREAK;
	}

	bool BaseGame::isClient() { return is_client; }

	void Initialize(BaseGame& game) {
		game_ptr = &game;

		SDL_Init(SDL_INIT_TIMER | SDL_INIT_EVENTS);
		sprintf(version_str, "%d.%d.%d", RG_VERSION_MAJ, RG_VERSION_MIN, RG_VERSION_PATCH);
		sprintf(platform_str, "%s", SDL_GetPlatform());

		if(game_ptr->isClient()) {
			SDL_InitSubSystem(RG_SDL_INIT_CLIENT_FLAGS);
		}

		Logger::Initialize();
		rgLogInfo(RG_LOG_SYSTEM, "Initializing engine...");
		SDL_SetAssertionHandler(AssertionHandler, NULL);

#ifdef RG_PLATFORM_WINDOWS
		DWORD username_len = 64;
		//GetUserName(user_profile_str, &username_len);
		sprintf(user_profile_str, "%s", " ~ NOT IMPLEMENTED YET ~");
#else
		sprintf(user_profile_str, "%s", getenv("LOGNAME"));
#endif

		// Checking architecture
		if(sizeof(void*) == 8) {
			is_arch64 = true;
		}

		rgLogInfo(RG_LOG_SYSTEM, "Username: %s", user_profile_str);

		SDL_version ver;
		SDL_GetVersion(&ver);

		rgLogInfo(RG_LOG_SYSTEM, "Engine version: %d.%d.%d, Build: %d", RG_VERSION_MAJ, RG_VERSION_MIN, RG_VERSION_PATCH, RG_BUILD);
		rgLogInfo(RG_LOG_SYSTEM, "SDL version: %d.%d.%d", ver.major, ver.minor, ver.patch);
		rgLogInfo(RG_LOG_SYSTEM, "Platform: %s, %s CPU, line %d, %d Mb ram.",
				SDL_GetPlatform(),
				Helper::getCpuName(SDL_GetCPUCount()),
				SDL_GetCPUCacheLineSize(),
				SDL_GetSystemRAM());

		if(is_debug) {
			rgLogWarn(RG_LOG_SYSTEM, " ~ ");
			rgLogWarn(RG_LOG_SYSTEM, " ~ Engine started in DEBUG profile!");
			rgLogWarn(RG_LOG_SYSTEM, " ~ ");
		}

		Command::Initialize();
		Filesystem::Initialize();
		profiler = new Profiler();

		Event::Initialize();
		Event::RegisterEventHandler(MainHandler);
		Input::Initialize();

		if(game_ptr->isClient()) {
			Window::Initialize();
			Sound::Initialize();
		}

		Thread::Initialize();
		if(game_ptr->isClient()) {
			Window::Show();
			sprintf(opengl_str, "%s %s", glGetString(GL_RENDERER), glGetString(GL_VERSION));
			rgLogInfo(RG_LOG_RENDER, "OpenGL: %s", GetGLInfo());
			Render::Initialize();
		} else {
			sprintf(opengl_str, "- (no renderer)");
		}

		game_ptr->Initialize();
	}

	void Start() {
		rgLogInfo(RG_LOG_SYSTEM, "Starting engine...");

		Thread::StartJobs();

		timer.Update();
		while (running) {
			profiler->Reset();
			frame_time = timer.GetElapsedTime();
			timer.Update();
			uptime += frame_time;

			profiler->Begin(RG_PROFILER_SEGMENT_wait_jobs);
			Thread::WaitJobs();
			Thread::StartJobs();

			profiler->Begin(RG_PROFILER_SEGMENT_main_update);
			game_ptr->MainUpdate();
			if(game_ptr->isClient()) {
				profiler->Begin(RG_PROFILER_SEGMENT_sound);
				Sound::Update();
				profiler->Begin(RG_PROFILER_SEGMENT_window_update);
				Window::Update();
			}
			profiler->Begin(RG_PROFILER_SEGMENT_process_events);
			Event::HandleEvents();
			Input::Update();
			profiler->Begin(RG_PROFILER_SEGMENT_other);
		}

		rgLogInfo(RG_LOG_SYSTEM, "Stopping engine...");

		if(game_ptr->isClient()) {
			Render::Destroy();
			Sound::Destroy();
			Window::Destroy();
		}

		rgLogInfo(RG_LOG_SYSTEM, "Game: Quit");
		game_ptr->Quit();
		Thread::Destroy();
		Input::Destroy();
		Event::Destroy();
		Filesystem::Destroy();
		delete profiler;

		rgLogInfo(RG_LOG_SYSTEM, "Engine: Quit");
		Command::Destroy();
		Logger::Destroy();
		SDL_Quit();

	}

	void Quit() {
		running = false;
	}

	void ForceQuit() {
		Logger::Destroy();
		exit(-1);
	}

	static void RG_LogState() {

		rgLogError(RG_LOG_ERROR, "~ ~ ~ SYSTEM ~ ~ ~");
		rgLogError(RG_LOG_ERROR, "Engine version: %d.%d.%d", RG_VERSION_MAJ, RG_VERSION_MIN, RG_VERSION_PATCH);
		rgLogError(RG_LOG_ERROR, "Rendering backend: %s", GetGLInfo());
		rgLogError(RG_LOG_ERROR, "Platform: %s", GetEnginePlatform());

	}

	void Error(String error) {
		char message[256];
		memset(message, 0, 256);
		sprintf(message, "Error: %s", error); // TODO

		String ERR_MESSAGE = "This is rgEngine has crashed. To help development process, please\n"
							"submit bug or save report and email it manually (button More...).\n"
							"Many thanks in advance and sorry for inconvenience.";
		// Print crash report
		rgLogError(RG_LOG_ERROR, " ");
		rgLogError(RG_LOG_ERROR, " ~ ~ ~ rgEngine crash report ~ ~ ~");
		rgLogError(RG_LOG_ERROR, " ");
		rgLogError(RG_LOG_ERROR, "%s", ERR_MESSAGE);
		rgLogError(RG_LOG_ERROR, " ");
		rgLogError(RG_LOG_ERROR, "Error: %s", error);
		rgLogError(RG_LOG_ERROR, " ");

		RG_LogState();

		if(game_ptr->isClient()) {
			const SDL_MessageBoxButtonData buttons[] = {
				{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "OK" },
				{ 0, 1, "More..." }
			};
			const SDL_MessageBoxData messageboxdata = { // @suppress("Invalid arguments")
				SDL_MESSAGEBOX_ERROR, Engine::Window::GetWindow(),
				"rgEngine has crashed!",
				ERR_MESSAGE,
				SDL_arraysize(buttons), buttons, NULL
			};

			int buttonid;
			SDL_ShowMessageBox(&messageboxdata, &buttonid);

			if (buttonid == 1) {
				HandleError(message);
			}
		}

		ForceQuit();
	}

	int ProcessArguments(int argc, String* argv) {
		for (int i = 0; i < argc; ++i) {
			if(Helper::streql(argv[i], "-debug")) {
				is_debug = true;
			} else if(Helper::streql(argv[i], "-t")) {
				if(argc <= i + 1) {
					return -1;
				}
				num_threads = atoi(argv[i + 1]);
			}
		}
		return 0;
	}

	void HandleError(String message) {
//		rgLogError(RG_LOG_SYSTEM, " ~ ~ ~ ~ ~\n");
		rgLogError(RG_LOG_SYSTEM, "Error: %s\n", message);

		if (game_ptr->isClient()) {
			SDL_ShowSimpleMessageBox(
				SDL_MESSAGEBOX_ERROR, "Error",
				message, Engine::Window::GetWindow());
		}

		ForceQuit();
	}

	bool IsArch64() {
		return is_arch64;
	}

	bool IsDebug() {
		return is_debug;
	}

	Uint32 GetThreads() {
		return num_threads;
	}

	double GetDeltaTime() {
		return frame_time;
	}

	double GetUptime() {
		return uptime;
	}

	BaseGame* GetGame() {
		return game_ptr;
	}

	Profiler* GetProfiler() {
		return profiler;
	}

	int GetExitCode() {
		return exit_code;
	}

	String GetEngineVersion() {
		return version_str;
	}

	String GetEnginePlatform() {
		return platform_str;
	}

	String GetGLInfo() {
		return opengl_str;
	}

	String GetUsername() {
		return user_profile_str;
	}

	UTF8Decoder* GetUTF8Decoder() {
		return &utf8_decoder;
	}

	UTF8Encoder* GetUTF8Encoder() {
		return &utf8_encoder;
	}
}
