/*
 * source.cpp
 *
 *  Created on: Feb 26, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/sound/source.h>
#include <cstdio>

namespace Engine {
	namespace Sound {

		SoundSource::SoundSource() {
			this->buffer = nullptr;
			this->pos = {0, 0, 0};
			alGenSources(1, &this->source);
		}

		SoundSource::~SoundSource() {
			alDeleteSources(1, &this->source);
		}

		void SoundSource::SetBuffer(SoundBuffer* buffer) {
			if(this->buffer != NULL) {
				this->buffer->SetSource(0);
			}
			this->buffer = buffer;
//			alSourcei(this->source, AL_BUFFER, buffer->GetBuffer());
			alSourcef(this->source, AL_ROLLOFF_FACTOR, 1.5f);
			this->buffer->SetSource(this->source);
		}

		SoundBuffer* SoundSource::GetBuffer() {
			return this->buffer;
		}

		void SoundSource::SetVolume(float v) {
			this->volume = v;
			alSourcef(this->source, AL_GAIN, v);
		}

		float SoundSource::GetVolume() {
			return this->volume;
		}

		void SoundSource::SetPosition(const Engine::Math::vec3& pos) {
			this->pos = pos;
		}

		Engine::Math::vec3* SoundSource::GetPosition() {
			return &this->pos;
		}

		ALuint SoundSource::GetSource() {
			return this->source;
		}

		void SoundSource::Play() {
			alSourcePlay(this->source);
		}

		void SoundSource::Stop() {
			alSourceStop(this->source);
		}

		void SoundSource::Pause() {
			alSourcePause(this->source);
		}

		bool SoundSource::IsPlaying() {
			ALint state;
			alGetSourcei(this->source, AL_SOURCE_STATE, &state);
			return state == AL_PLAYING;
		}

		void SoundSource::SetRepeat(bool r) {
//			alSourcei(this->source, AL_LOOPING, r ? AL_TRUE : AL_FALSE);
		}

	}
}
