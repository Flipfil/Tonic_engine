#include "tonic/audio/sound.h"

#include "tonic/log.h"

#include "SDL2/SDL_Audio.h"

namespace tonic::audio
{
	Sound::Sound(const std::string& file_name)
	{
		m_audio_spec = new SDL_AudioSpec;
		if (SDL_LoadWAV(file_name.c_str(), m_audio_spec, &m_wave_start, &m_wave_lenght) == nullptr)
		{
			TONIC_ERROR("Sound::Sound - loading error. Make sure file exists and has .wav format: {}", SDL_GetError());
		}
	}

	Sound::~Sound()
	{
		delete m_audio_spec;
		SDL_FreeWAV(m_wave_start);
		SDL_CloseAudioDevice(m_device_ID);
	}

	void Sound::Play()
	{
		int status = SDL_QueueAudio(m_device_ID, m_wave_start, m_wave_lenght);
		SDL_PauseAudioDevice(m_device_ID, 0);
	}

	void Sound::Stop()
	{
		SDL_PauseAudioDevice(m_device_ID, 1);
	}

	void Sound::SetupDevice()
	{
		m_device_ID = SDL_OpenAudioDevice(nullptr, 0, m_audio_spec, nullptr, 0);
		if (m_device_ID == 0)
		{
			TONIC_ERROR("Sound::SetupDevice - error while creating device: {}", SDL_GetError());
		}
	}
}