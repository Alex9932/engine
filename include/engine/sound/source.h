/*
 * source.h
 *
 *  Created on: Feb 26, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_SOUND_SOURCE_H_
#define ENGINE_SOUND_SOURCE_H_

#include <AL/al.h>
#include <engine/sound/buffer.h>
#include <engine/core/math/vector.h>

namespace Engine {
	namespace Sound {

		class RG_DECLSPEC SoundSource {
			private:
				ALuint source;
				SoundBuffer* buffer;
				Engine::Math::vec3 pos;
				float volume = 1;

			public:
				SoundSource();
				virtual ~SoundSource();
				void SetBuffer(SoundBuffer* buffer);
				SoundBuffer* GetBuffer();
				void SetVolume(float v);
				float GetVolume();
				void SetPosition(const Engine::Math::vec3& pos);
				Engine::Math::vec3* GetPosition();

				ALuint GetSource();

				void Play();
				void Stop();
				void Pause();
				bool IsPlaying();
				void SetRepeat(bool r);

		};

	}
}


#endif /* ENGINE_SOUND_SOURCE_H_ */
