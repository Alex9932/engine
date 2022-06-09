/*
 * thread.cpp
 *
 *  Created on: Feb 11, 2022
 *      Author: alex9932
 */

#define _CRT_SECURE_NO_WARNINGS
#define DLL_EXPORT

#include <engine/core/thread/thread.h>
#include <cstdio>

#define MAX_TASKS 1024

namespace Engine {
	namespace Thread {

		static Thread** threads_pool;
		static Uint32 threads;

		static Worker* tasks[MAX_TASKS];
		static void* tasks_data[MAX_TASKS];

		static int THR_Function(void* data) {
			Thread* thread = (Thread*)data;
			return thread->Run();
		}

		static Uint32 task_id = 0;
		static SDL_mutex* mutex;
		static Task RetrieveTask() {
			Task t = -1;
			if (SDL_LockMutex(mutex) == 0) {
				for (Uint32 i = task_id; i < MAX_TASKS; ++i) {
					if(tasks[i] == NULL) { continue; }

					if(tasks[i]->GetType() == RG_WORKER_EXECUTE_ALWAYS ||
						(tasks[i]->GetType() == RG_WORKER_EXECUTE_ONCE && tasks[i]->IsExecute())
					) {
						t = i;
						break;
					}
				}
				task_id++;
				SDL_UnlockMutex(mutex);
			} else {
				rgLogInfo(RG_LOG_SYSTEM, "Couldn't lock mutex\n");
			}
			return t;
		}

		Thread::Thread(Uint32 id) {
			memset(this->name, 0, 32);
			sprintf(this->name, "worker %d", id);
		}

		Thread::~Thread() {

		}

		int Thread::Run() {
			Task task;
			while ((task = RetrieveTask()) != -1) {
				tasks[task]->SetExecuted();
				tasks[task]->DoWork(tasks_data[task]);
			}

			return 0;
		}

		void Thread::Start() {
			this->thread = SDL_CreateThread(THR_Function, this->name, this);
		}

		int Thread::Wait() {
			int exit_code;
			SDL_WaitThread(this->thread, &exit_code);
			return exit_code;
		}

		String Thread::GetName() {
			return this->name;
		}

		void Initialize() {
			mutex = SDL_CreateMutex();
			memset(tasks, 0, sizeof(Task) * MAX_TASKS);
			threads = Engine::GetThreads();
			threads_pool = (Thread**)malloc(sizeof(Thread*) * threads);
			for (Uint32 i = 0; i < threads; ++i) {
				threads_pool[i] = new Thread(i);
			}
		}

		void Destroy() {
			SDL_DestroyMutex(mutex);
			for (Uint32 i = 0; i < threads; ++i) {
				delete threads_pool[i];
			}
			free(threads_pool);
		}

		void StartJobs() {
			task_id = 0;
			for (Uint32 i = 0; i < threads; ++i) {
				threads_pool[i]->Start();
			}
		}

		void WaitJobs() {
			for (Uint32 i = 0; i < threads; ++i) {
				int e_code = threads_pool[i]->Wait();
				if(e_code != 0) {
					Engine::Error("thread error");
				}
			}

			for (Uint32 i = 0; i < MAX_TASKS; ++i) {
				if(tasks[i] == NULL) { continue; }
//				rgLogInfo(RG_LOG_SYSTEM, "Thread %d: SYNC", i);
				tasks[i]->Sync(tasks_data[i]);
			}
		}

		Task RegisterTask(Worker* worker, void* data) {
			Task task_id = -1;

			for (Uint32 i = 0; i < MAX_TASKS; ++i) {
				if(tasks[i] == NULL) {
					tasks[i] = worker;
					tasks_data[i] = data;
					task_id = i;
					break;
				}
			}

			return task_id;
		}

		void FreeTask(Task task) {
			tasks[task] = NULL;
		}

	}
}
