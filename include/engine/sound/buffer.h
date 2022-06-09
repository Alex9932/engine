/*
 * buffer.h
 *
 *  Created on: Feb 26, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_SOUND_BUFFER_H_
#define ENGINE_SOUND_BUFFER_H_

#define RG_SND_BUFFER_SIZE 4096*64
//#define RG_SND_BUFFER_SIZE 1024

#include <AL/al.h>
#include <engine/sound/sound_loader.h>

namespace Engine {
	namespace Sound {

		class RG_DECLSPEC SoundBuffer {
			private:
				ALuint buffer[2];
				ALuint source;
				ALuint currentBuffer;
				SoundData data;
				short* buf;
				Uint32 ptr = 0;
				bool stream = true;
				bool loop = true;

			public:
				SoundBuffer(SoundData data);
				virtual ~SoundBuffer();
				ALuint GetBuffer();
				void SetSource(ALuint source);
				void Update();
		};

	}
}

#endif /* ENGINE_SOUND_BUFFER_H_ */
