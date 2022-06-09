/*
 * allocator.cpp
 *
 *  Created on: Apr 12, 2022
 *      Author: alex9932
 */
#define DLL_EXPORT
#include <engine/core/allocator.h>

#define RG_MAX_ALLOCATORS 256

namespace Engine {

	static Allocator* allocators[RG_MAX_ALLOCATORS];
	static Uint32 ptr = 0;

	Allocator::Allocator(String name) { this->name = name; }
	Allocator::~Allocator() {}
	String Allocator::GetName() { return name; }
	void* Allocator::Allocate(size_t len) { return NULL; }
	void Allocator::Deallocate(void* ptr) {}
	size_t Allocator::GetAllocatedMemory() { return 0; }

	STDAllocator::STDAllocator(String name): Allocator(name) {}
	STDAllocator::~STDAllocator() {}

	void* STDAllocator::Allocate(size_t len) {
		AllocBlock block;
		block.len = len;
		block.addr = malloc(len);
//		rgLogError(RG_LOG_SYSTEM, "ALLOC (%s): New: 0x%x", name, block.addr);
		blocks.push_back(block);
		return block.addr;
	}

	void STDAllocator::Deallocate(void* ptr) {
		for (std::vector<AllocBlock>::iterator it = blocks.begin(); it != blocks.end(); ++it) {
			if(it->addr == ptr) {
// TODO
//				if(Engine::IsArch64()) {
//					rgLogDebug(RG_LOG_SYSTEM, "ALLOC (%s): Free: 0x%lx", name, (Uint64)ptr);
//				} else {
//					rgLogDebug(RG_LOG_SYSTEM, "ALLOC (%s): Free: 0x%x", name, (Uint32)ptr);
//				}
				blocks.erase(it);
				free(ptr);
				return;
			}
		}

//		rgLogDebug(RG_LOG_SYSTEM, "ALLOC (%s): Alloc table %d:", name, blocks.size());
//		Uint32 i = 0;
//		for (std::vector<AllocBlock>::iterator it = blocks.begin(); it != blocks.end(); ++it) {
//			rgLogDebug(RG_LOG_SYSTEM, "ALLOC (%s):  %d : 0x%x", name, i, it->addr);
//			i++;
//		}
//		rgLogDebug(RG_LOG_SYSTEM, "ALLOC (%s): Messamtch pointer: 0x%x", name, ptr);

		RG_ERROR_MSG("STD_ALLOCATOR: Invalid pointer!");
	}

	size_t STDAllocator::GetAllocatedMemory() {
		size_t mem = 0;
		for (Uint32 i = 0; i < blocks.size(); ++i) {
			mem += blocks[i].len;
		}
		return mem;
	}

	void RegisterAllocator(Allocator* allocator) {
		if(ptr >= RG_MAX_ALLOCATORS) {
			RG_ASSERT_MSG(NULL, "Unable to register new allocator!");
			return;
		}
		allocators[ptr] = allocator;
		ptr++;
	}

	Allocator** GetAllocators() {
		return allocators;
	}

	Uint32 GetMaxAllocators() {
		return RG_MAX_ALLOCATORS;
	}

	size_t GetTotalAllocatedMemory() {
		size_t total = 0;
		for (Uint32 i = 0; i < RG_MAX_ALLOCATORS; ++i) {
			if(allocators[i] != NULL) {
				total += allocators[i]->GetAllocatedMemory();
			}
		}
		return total;
	}

}
