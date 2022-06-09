/*
 * sound.h
 *
 *  Created on: Feb 26, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_SOUND_SOUND_H_
#define ENGINE_SOUND_SOUND_H_

#include <engine/engine.h>
#include <engine/sound/buffer.h>

namespace Engine {
	namespace Sound {

		RG_DECLSPEC void Initialize();
		RG_DECLSPEC void Destroy();
		RG_DECLSPEC void Update();

		RG_DECLSPEC SoundBuffer* NewBuffer(String path);
		RG_DECLSPEC void DeleteBuffer(SoundBuffer* buffer);

	}
}


#endif /* ENGINE_SOUND_SOUND_H_ */
