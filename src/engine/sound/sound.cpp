/*
 * sound.cpp
 *
 *  Created on: Feb 26, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/sound/sound.h>
#include <engine/sound/sound_loader.h>
#include <engine/core/loader.h>
#include <engine/core/logger.h>

#include <AL/al.h>
#include <AL/alc.h>

#include <vector>

static ALCdevice* device;
static ALCcontext* context;

namespace Engine {
	namespace Sound {

		static OGGSoundLoader* ogg_loader;

		static std::vector<SoundBuffer*> buffers;

		void Initialize() {
			rgLogInfo(RG_LOG_AUDIO, "AL: Open device...");
			device = alcOpenDevice(0);
			context = alcCreateContext(device, NULL);
			if (!alcMakeContextCurrent(context)) {
				rgLogError(RG_LOG_AUDIO, "AL: Init failed!");
			}


			rgLogInfo(RG_LOG_AUDIO, "AL: Version: %s", alGetString(AL_VERSION));

			ogg_loader = new OGGSoundLoader();
		}

		void Destroy() {
			rgLogInfo(RG_LOG_AUDIO, "AL: Quit...");
			for (Uint32 i = 0; i < buffers.size(); ++i) {
				delete buffers[i];
			}
			buffers.clear();
			delete ogg_loader;
			alcDestroyContext(context);
			alcCloseDevice(device);
		}

		void Update() {
			for (Uint32 i = 0; i < buffers.size(); ++i) {
				buffers[i]->Update();
			}
		}

		SoundBuffer* NewBuffer(String path) {
			SoundData data = Engine::Loader::LoadSound(ogg_loader, path);
			SoundBuffer* sb = new SoundBuffer(data);
			buffers.push_back(sb);
			return sb;
		}

		void DeleteBuffer(SoundBuffer* buffer) {
			for (std::vector<SoundBuffer*>::iterator it = buffers.begin(); it != buffers.end(); ++it ) {
				if ( *it == buffer ) {
					buffers.erase(it);
				}
			}
			delete buffer;
//			Engine::Loader::FreeSound(buffer->GetBuffer());
			delete buffer;
		}

	}
}
