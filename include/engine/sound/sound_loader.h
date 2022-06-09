/*
 * loader.h
 *
 *  Created on: Feb 26, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_SOUND_LOADER_H_
#define ENGINE_SOUND_LOADER_H_

#include <AL/al.h>
#include <engine/engine.h>

namespace Engine {
	namespace Sound {

		typedef struct SoundData {
			short* data;
			int sample_rate;
			int channels;
			int rc;
			ALenum format;
		} SoundData;

		class RG_DECLSPEC SoundLoader {
			public:
				SoundLoader();
				virtual ~SoundLoader();
				virtual SoundData LoadSound(String path);
		};

		class RG_DECLSPEC OGGSoundLoader: public SoundLoader {
			public:
				OGGSoundLoader();
				virtual ~OGGSoundLoader();
				virtual SoundData LoadSound(String path);
		};

		class RG_DECLSPEC WAVSoundLoader: public SoundLoader {
			public:
				WAVSoundLoader();
				virtual ~WAVSoundLoader();
				virtual SoundData LoadSound(String path);
		};

	}
}

#endif /* ENGINE_SOUND_LOADER_H_ */
