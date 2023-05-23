#pragma once

#include "spdlog/spdlog.h"

#define TNK_DEFAULT_LOGGER_NAME "TNK_logger"
#ifdef TNK_CONFIG_DEBUG 

	#define TNK_TRACE(...)	if(spdlog::get(TNK_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(TNK_DEFAULT_LOGGER_NAME)->trace(__VA_ARGS__);}
	#define TNK_DEBUG(...)	if(spdlog::get(TNK_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(TNK_DEFAULT_LOGGER_NAME)->debug(__VA_ARGS__);}
	#define TNK_INFO(...)	if(spdlog::get(TNK_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(TNK_DEFAULT_LOGGER_NAME)->info(__VA_ARGS__);}
	#define TNK_WARN(...)	if(spdlog::get(TNK_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(TNK_DEFAULT_LOGGER_NAME)->warn(__VA_ARGS__);}
	#define TNK_ERROR(...)	if(spdlog::get(TNK_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(TNK_DEFAULT_LOGGER_NAME)->error(__VA_ARGS__);}
	#define TNK_FATAL(...)	if(spdlog::get(TNK_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(TNK_DEFAULT_LOGGER_NAME)->critical(__VA_ARGS__);}
	
#else

	#define TNK_TRACE(...)	(void)0
	#define TNK_DEBUG(...)	(void)0
	#define TNK_INFO(...)	(void)0
	#define TNK_WARN(...)	(void)0
	#define TNK_ERROR(...)	(void)0
	#define TNK_FATAL(...)	(void)0

#endif