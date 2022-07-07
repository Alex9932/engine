/*
 * thread.cpp
 *
 *  Created on: Feb 11, 2022
 *      Author: alex9932
 */

#define _CRT_SECURE_NO_WARNINGS
#define DLL_EXPORT

#include <engine/core/thread.h>
#include <cstdio>

#define MAX_TASKS 1024

namespace Engine {
	namespace Thread {

		static bool running = true;
		static Uint32 threads;
		static Thread** threads_pool;
		static bool* threads_idle;

		static Worker* tasks[MAX_TASKS];
		static void* tasks_data[MAX_TASKS];

		static Uint32 task_id = 0;
		static SDL_mutex* mutex;

		static int THR_Function(void* data) {
			Thread* thread = (Thread*)data;
			return thread->Run();
		}

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

		static void WaitThreads() {
			for (Uint32 i = 0; i < threads; ++i) {
				int e_code = threads_pool[i]->Wait();
				if(e_code != 0) {
					Engine::Error("thread error");
				}
			}

			for (Uint32 i = 0; i < MAX_TASKS; ++i) {
				if(tasks[i] == NULL) { continue; }
				tasks[i]->Sync(tasks_data[i]);
			}
		}

		void Initialize() {
			running = true;
			mutex = SDL_CreateMutex();
			memset(tasks, 0, sizeof(Task) * MAX_TASKS);
			threads = Engine::GetThreads();
			threads_idle = (bool*)malloc(sizeof(bool) * threads);
			threads_pool = (Thread**)malloc(sizeof(Thread*) * threads);
			for (Uint32 i = 0; i < threads; ++i) {
				threads_pool[i] = new Thread(i);
			}

			for (Uint32 i = 0; i < threads; ++i) {
				threads_pool[i]->Start();
			}
		}

		void Destroy() {
			running = false;
			WaitThreads();

			SDL_DestroyMutex(mutex);
			for (Uint32 i = 0; i < threads; ++i) {
				delete threads_pool[i];
			}
			free(threads_pool);
			free(threads_idle);
		}

		void StartJobs() {
			task_id = 0;
		}

		void WaitJobs() {
			while (true) {
				bool b = true;
				for (Uint32 i = 0; i < threads; ++i) {
					if(!threads_idle[i]) { b = false; }
				}
				if(b) { break; }
			}

			for (Uint32 i = 0; i < MAX_TASKS; ++i) {
				if(tasks[i] == NULL) { continue; }
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


		// Worker class
		Worker::Worker()  {}
		Worker::~Worker() {}
		int    Worker::DoWork     (void* data)  { return 0; }
		int    Worker::Sync       (void* data)  { return 0; }
		void   Worker::SetType    (Uint32 type) { this->type = type; }
		Uint32 Worker::GetType    ()            { return this->type; }
		bool   Worker::IsExecute  ()            { return this->execute; }
		void   Worker::Shedule    ()            { this->execute = true; }
		void   Worker::SetExecuted()            { this->execute = false; }


		// Thread class
		Thread::Thread(Uint32 id) {
			memset(this->name, 0, 32);
			sprintf(this->name, "worker %d", id);
			this->id = id;
		}

		Thread::~Thread() {}

		int Thread::Run() {
			Task task;
			while (running) {
				if((task = RetrieveTask()) != -1) {
					threads_idle[this->id] = false;
					tasks[task]->SetExecuted();
					tasks[task]->DoWork(tasks_data[task]);
				} else {
					threads_idle[this->id] = true;
				}
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

	}
}
