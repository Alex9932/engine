/*
 * allocator.h
 *
 *  Created on: Apr 12, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_CORE_ALLOCATOR_H_
#define ENGINE_CORE_ALLOCATOR_H_

#include <engine/engine.h>
#include <vector>

namespace Engine {

	struct AllocBlock {
		size_t len;
		void* addr;
	};

	class RG_DECLSPEC Allocator {
		protected:
			String name;

		public:
			Allocator(String name);
			virtual ~Allocator();
			virtual void* Allocate(size_t len);
			virtual void Deallocate(void* ptr);
			virtual size_t GetAllocatedMemory();
			String GetName();
	};

	class RG_DECLSPEC STDAllocator: public Allocator {
		private:
			std::vector<AllocBlock> blocks;

		public:
			STDAllocator(String name);
			virtual ~STDAllocator();
			virtual void* Allocate(size_t len);
			virtual void Deallocate(void* ptr);
			virtual size_t GetAllocatedMemory();
	};

	RG_DECLSPEC void RegisterAllocator(Allocator* allocator);
	RG_DECLSPEC Allocator** GetAllocators();
	RG_DECLSPEC Uint32 GetMaxAllocators();
	RG_DECLSPEC size_t GetTotalAllocatedMemory();

}


#endif /* ENGINE_CORE_ALLOCATOR_H_ */
