#pragma once

#include "spdlog/spdlog.h"

#define TONIC_DEFAULT_LOGGER_NAME "TONIC_logger"
#ifdef TONIC_CONFIG_DEBUG 

	#define TONIC_TRACE(...)	if(spdlog::get(TONIC_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(TONIC_DEFAULT_LOGGER_NAME)->trace(__VA_ARGS__);}
	#define TONIC_DEBUG(...)	if(spdlog::get(TONIC_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(TONIC_DEFAULT_LOGGER_NAME)->debug(__VA_ARGS__);}
	#define TONIC_INFO(...)	if(spdlog::get(TONIC_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(TONIC_DEFAULT_LOGGER_NAME)->info(__VA_ARGS__);}
	#define TONIC_WARN(...)	if(spdlog::get(TONIC_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(TONIC_DEFAULT_LOGGER_NAME)->warn(__VA_ARGS__);}
	#define TONIC_ERROR(...)	if(spdlog::get(TONIC_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(TONIC_DEFAULT_LOGGER_NAME)->error(__VA_ARGS__);}
	#define TONIC_FATAL(...)	if(spdlog::get(TONIC_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(TONIC_DEFAULT_LOGGER_NAME)->critical(__VA_ARGS__);}
	
#else

	#define TONIC_TRACE(...)	(void)0
	#define TONIC_DEBUG(...)	(void)0
	#define TONIC_INFO(...)		(void)0
	#define TONIC_WARN(...)		(void)0
	#define TONIC_ERROR(...)	(void)0
	#define TONIC_FATAL(...)	(void)0

#endif

#ifdef TONIC_PLATFORM_WINDOWS
#define TONIC_BREAK __debugbreak();

#elif TONIC_PLATFORM_MACOS
#define TONIC_BREAK __builtin_debugtrap();

#else TONIC_PLATFORM_LINUX
#define TONIC_BREAK __builtin_trap();

#endif


#ifdef TONIC_CONFIG_DEBUG
#define TONIC_ASSERT(x, msg) if((x)) {} else { TONIC_FATAL("ASSERT - {}\n\t{}\n\tin file: {}\n\ton line: {}", #x, msg, __FILE__, __LINE__); TONIC_BREAK}
#else
#define TONIC_ASSERT(x, msg) (void)0
#endif
