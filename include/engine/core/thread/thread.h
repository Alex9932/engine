/*
 * thread.h
 *
 *  Created on: Feb 11, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_CORE_THREAD_THREAD_H_
#define ENGINE_CORE_THREAD_THREAD_H_


#include <engine/engine.h>
#include <engine/core/thread/worker.h>
#include <SDL2/SDL.h>

namespace Engine {
	namespace Thread {

		typedef Sint32 Task;

		class RG_DECLSPEC Thread {
			private:
				char name[32];
				SDL_Thread* thread = NULL;

			public:
				Thread(Uint32 id);
				~Thread();

				void Start();
				int Run();
				int Wait();
				String GetName();
		};

		RG_DECLSPEC void Initialize();
		RG_DECLSPEC void Destroy();

		RG_DECLSPEC void StartJobs();
		RG_DECLSPEC void WaitJobs();

		RG_DECLSPEC Task RegisterTask(Worker* worker, void* data);
		RG_DECLSPEC void FreeTask(Task task);

	}
}


#endif /* ENGINE_CORE_THREAD_THREAD_H_ */
