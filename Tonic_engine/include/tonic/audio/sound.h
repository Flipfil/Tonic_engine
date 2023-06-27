#pragma once

#include <string>

struct SDL_AudioSpec;
namespace tonic::audio
{
	void PlaySound(const std::string& file_name);

	// abstract interface
	class ISound
	{
	public:
		virtual ~ISound() {};

		virtual void Play() = 0;
		virtual void Stop() = 0;
	};

	// current implementation
	class Sound : public ISound
	{
	public:
		Sound(const std::string& file_name);
		~Sound();

		void Play() override;
		void Stop() override;
		void SetupDevice();

	private:
		uint32_t m_device_ID;
		SDL_AudioSpec* m_audio_spec;
		uint8_t* m_wave_start;
		uint32_t m_wave_lenght;
	};
}