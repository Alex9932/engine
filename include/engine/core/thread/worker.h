/*
 * worker.h
 *
 *  Created on: Mar 3, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_CORE_THREAD_WORKER_H_
#define ENGINE_CORE_THREAD_WORKER_H_

#include <SDL2/SDL.h>
#include <engine/engine.h>

#define RG_WORKER_EXECUTE_ALWAYS 0
#define RG_WORKER_EXECUTE_ONCE   1

namespace Engine {
	namespace Thread {

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

	}
}

#endif /* ENGINE_CORE_THREAD_WORKER_H_ */
