/*
 * thread.h
 *
 *  Created on: Jul 6, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_CORE_THREAD_H_
#define ENGINE_CORE_THREAD_H_


#define RG_WORKER_EXECUTE_ALWAYS 0
#define RG_WORKER_EXECUTE_ONCE   1

#include <engine/engine.h>
#include <engine/core/thread/worker.h>
#include <SDL2/SDL.h>

namespace Engine {
	namespace Thread {

		typedef Sint32 Task;

		class RG_DECLSPEC Worker {
			private:
				Uint32 type = RG_WORKER_EXECUTE_ALWAYS;
				bool execute = false; // Used only if type == RG_WORKER_EXECUTE_ONCE

			public:
				Worker();
				virtual ~Worker();
				virtual int DoWork(void* data);
				virtual int Sync(void* data);

				void SetType(Uint32 type);
				Uint32 GetType();

				bool IsExecute();
				void Shedule();
				void SetExecuted();
		};

		class RG_DECLSPEC Thread {
			private:
				char name[32];
				Uint32 id;
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

#endif /* ENGINE_CORE_THREAD_H_ */
