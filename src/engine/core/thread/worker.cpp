/*
 * worker.cpp
 *
 *  Created on: Mar 3, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/core/thread/worker.h>

namespace Engine {
	namespace Thread {

		// Empty class
		Worker::Worker()  {}
		Worker::~Worker() {}

		int    Worker::DoWork     (void* data)  { return 0; }
		int    Worker::Sync       (void* data)  { return 0; }
		void   Worker::SetType    (Uint32 type) { this->type = type; }
		Uint32 Worker::GetType    ()            { return this->type; }
		bool   Worker::IsExecute  ()            { return this->execute; }
		void   Worker::Shedule    ()            { this->execute = true; }
		void   Worker::SetExecuted()            { this->execute = false; }

	}
}
