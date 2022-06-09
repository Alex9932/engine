/*
 * loader.cpp
 *
 *  Created on: Feb 26, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/sound/sound_loader.h>
#include <engine/core/logger.h>
#include <engine/core/filesystem.h>
#include <stb/stb_vorbis.h>

namespace Engine {
	namespace Sound {
		SoundLoader::SoundLoader() {}
		SoundLoader::~SoundLoader() {}
		SoundData SoundLoader::LoadSound(String path) {
			SoundData data;
			return data;
		}

		OGGSoundLoader::OGGSoundLoader() {}
		OGGSoundLoader::~OGGSoundLoader() {}
		SoundData OGGSoundLoader::LoadSound(String path) {
			SoundData data;
			rgLogInfo(RG_LOG_AUDIO, "AL: Loading sound: %s", path);
			Engine::Filesystem::Resource* res = Engine::Filesystem::GetResource(path);
			data.rc = stb_vorbis_decode_memory((const unsigned char*)res->data, res->length, &data.channels, &data.sample_rate, &data.data);
			if (data.rc == -1) {
				rgLogInfo(RG_LOG_AUDIO, "AL: Unable to load file!");
			}
			if (data.channels == 1) { data.format = AL_FORMAT_MONO16; }
			else { data.format = AL_FORMAT_STEREO16; }
			Engine::Filesystem::FreeResource(res);
			return data;
		}

		WAVSoundLoader::WAVSoundLoader() {}
		WAVSoundLoader::~WAVSoundLoader() {}
		SoundData WAVSoundLoader::LoadSound(String path) {
			// Not implemented yet!
			SoundData data;
			return data;
		}

	}
}
