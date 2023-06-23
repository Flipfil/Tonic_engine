#include "tonic/managers/logmanager.h"
#include "tonic/log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

#include <memory>
#include <vector>

namespace tonic::managers
{
	//--------------------------------

	void LogManager::Initialize()
	{
		auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		console_sink->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] %v%$");
	
		std::vector<spdlog::sink_ptr> sinks{ console_sink };
		auto logger = std::make_shared<spdlog::logger>(TONIC_DEFAULT_LOGGER_NAME, sinks.begin(), sinks.end());
		logger->set_level(spdlog::level::trace);
		logger->flush_on(spdlog::level::trace);
		spdlog::register_logger(logger);
	}

	//--------------------------------

	void LogManager::Shutdown()
	{
		spdlog::shutdown();
	}
}