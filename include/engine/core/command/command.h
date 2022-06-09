/*
 * command.h
 *
 *  Created on: Apr 3, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_CORE_COMMAND_COMMAND_H_
#define ENGINE_CORE_COMMAND_COMMAND_H_

#include <engine/engine.h>
#include <vector>

namespace Engine {
	namespace Command {

		RG_DECLSPEC void Initialize();
		RG_DECLSPEC void Destroy();

		RG_DECLSPEC void ExecuteCommand(String str);

		RG_DECLSPEC void RegisterCommand(String cmd);
		RG_DECLSPEC std::vector<String>& GetCommansList();

	}
}

#endif /* ENGINE_CORE_COMMAND_COMMAND_H_ */
