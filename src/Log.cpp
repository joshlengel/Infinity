#include"Log.h"

#include<spdlog/sinks/stdout_color_sinks.h>

namespace Infinity
{
	std::shared_ptr<spdlog::logger> Log::sp_core_logger;
	std::shared_ptr<spdlog::logger> Log::sp_client_logger;

	Pointer<spdlog::logger> Log::core_logger;
	Pointer<spdlog::logger> Log::client_logger;

	void Log::Init()
	{
		sp_core_logger = spdlog::stdout_color_mt("Infinity");
		sp_client_logger = spdlog::stdout_color_mt("Client");

		core_logger = &sp_core_logger;
		client_logger = &sp_client_logger;

		core_logger->set_level(spdlog::level::trace);
		client_logger->set_level(spdlog::level::trace);

		core_logger->set_pattern("%^[%T] %n: %v%$");
		client_logger->set_pattern("%^[%T] %n: %v%$");
	}

	Pointer<spdlog::logger> Log::GetCoreLogger()
	{
		return core_logger;
	}

	Pointer<spdlog::logger> Log::GetClientLogger()
	{
		return client_logger;
	}
}