/*
 * buffer.cpp
 *
 *  Created on: Feb 26, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/sound/buffer.h>
#include <engine/core/loader.h>

namespace Engine {
	namespace Sound {

		SoundBuffer::SoundBuffer(SoundData data) {
			this->data = data;
			this->source = 0;
			this->buf = (short*)malloc(sizeof(short) * RG_SND_BUFFER_SIZE);
			this->currentBuffer = 0;

			alGenBuffers(2, buffer);

//			printf("RC: %d\n", data.rc);

			for (Uint32 i = 0; i < 2; ++i) {
				int size = SDL_min(RG_SND_BUFFER_SIZE, data.rc*sizeof(short) - ptr);
				SDL_memcpy(buf, &data.data[ptr], size * sizeof(short));
//				SDL_memcpy(buf, &data.data[ptr], sizeof(short) * RG_SND_BUFFER_SIZE);
				ptr += size;
				alBufferData(buffer[i], data.format, buf, sizeof(short) * RG_SND_BUFFER_SIZE, data.sample_rate);
			}

//			alSourceUnqueueBuffers(source, 1, &buffer[0]);

		}

		SoundBuffer::~SoundBuffer() {
			free(this->buf);
			Engine::Loader::FreeSound(data);
			alDeleteBuffers(2, buffer);
		}

		ALuint SoundBuffer::GetBuffer() {
			return this->buffer[currentBuffer];
		}

		void SoundBuffer::SetSource(ALuint src) {
			this->source = src;
//			printf("AL: Queued %d %d\n", source, buffer[0]);
			alSourceQueueBuffers(source, 2, buffer);
		}

		void SoundBuffer::Update() {
			if(this->stream) {
				ALint processed;
				alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
				if(processed == 0) { return; }

//				printf("AL: Update\n");
				if(processed == 1) {
					alSourceUnqueueBuffers(source, 1, &buffer[currentBuffer]);

					int size = SDL_min(RG_SND_BUFFER_SIZE, data.rc*sizeof(short) - ptr);
					SDL_memcpy(buf, &data.data[ptr], size * sizeof(short));
					alBufferData(buffer[currentBuffer], data.format, buf, size * sizeof(short), data.sample_rate);
					ALenum error = alGetError();
					if (error != 0) {
						rgLogInfo(RG_LOG_AUDIO, "AL: Error code: %d", error);
					}

					ptr += RG_SND_BUFFER_SIZE;
					if(ptr > data.rc * sizeof(short)) {
						if(loop) {
							ptr = 0;
						} else {
							alSourceUnqueueBuffers(source, 2, buffer);
							return;
						}
					}

					alSourceQueueBuffers(source, 1, &buffer[currentBuffer]);
					currentBuffer = 1 - currentBuffer;
				} else {
					if(processed == 2) {
						alSourceUnqueueBuffers(source, 2, buffer);
					}
				}
			}
		}

	}
}
