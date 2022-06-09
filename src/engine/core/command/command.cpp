/*
 * command.cpp
 *
 *  Created on: Apr 3, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/core/command/command.h>
#include <engine/core/strings.h>
#include <engine/core/event.h>
#include <engine/core/allocator.h>

#define RG_COMMAND_QUIT_STR       "quit"
#define RG_COMMAND_RG_INFO_STR    "rg_info"
#define RG_COMMAND_RG_MEMDUMP_STR "rg_memdump"
#define RG_COMMAND_RG_TESTMSG_STR "rg_testmsg"

namespace Engine {
	namespace Command {

		std::vector<String> commands;

		void Initialize() {
			RegisterCommand(RG_COMMAND_QUIT_STR);
			RegisterCommand(RG_COMMAND_RG_INFO_STR);
			RegisterCommand(RG_COMMAND_RG_MEMDUMP_STR);
			RegisterCommand(RG_COMMAND_RG_TESTMSG_STR);
		}

		void Destroy() {

		}

		void ExecuteCommand(String str) {
			if(Helper::streql(str, RG_COMMAND_QUIT_STR)) {
				Quit();
			} else if(Helper::streql(str, RG_COMMAND_RG_INFO_STR)) {
				rgLogInfo(RG_LOG_SYSTEM, "rg_engine v%s on %s", GetEngineVersion(), GetEnginePlatform());
				rgLogInfo(RG_LOG_SYSTEM, "OpenGL: %s", GetGLInfo());
			} else if(Helper::streql(str, RG_COMMAND_RG_MEMDUMP_STR)) {
				rgLogInfo(RG_LOG_SYSTEM, "rg_engine Memory allocators info");
				Allocator** allocators = GetAllocators();
				for (Uint32 i = 0; i < GetMaxAllocators(); ++i) {
					Allocator* allocator = allocators[i];
					if(allocator != NULL) {
						rgLogInfo(RG_LOG_SYSTEM, "%s : allocated %ldKb", allocator->GetName(), allocator->GetAllocatedMemory()/1000);
					}
				}
				rgLogInfo(RG_LOG_SYSTEM, "~ ~ ~ Summary ~ ~ ~");
				rgLogInfo(RG_LOG_SYSTEM, "Total allocated: %ldKb", GetTotalAllocatedMemory()/1000);
			} else if(Helper::streql(str, RG_COMMAND_RG_TESTMSG_STR)) {

				rgLogCritical(RG_LOG_SYSTEM, "Critical error message");
				rgLogError(RG_LOG_SYSTEM, "Error message");
				rgLogWarn(RG_LOG_SYSTEM, "Warning message");
				rgLogInfo(RG_LOG_SYSTEM, "Info message");
				rgLogDebug(RG_LOG_SYSTEM, "Debug message");

			}

			Engine::Event::PushEvent(0xFFFFFFFF, RG_EVENT_COMMAND, (void*)str, NULL);
		}

		void RegisterCommand(String cmd) {
			commands.push_back(cmd);
		}

		std::vector<String>& GetCommansList() {
			return commands;
		}

	}
}
