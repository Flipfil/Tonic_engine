#pragma once

namespace tonic::managers
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