#pragma once


namespace TNK::MANAGERS
{
	class LogManager
	{
	public:
		LogManager() = default;
		~LogManager() = default;

		void Initialize();
		void Shutdown();
	};
}