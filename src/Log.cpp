#include"InfinityPCH.h"

#ifdef DEBUG
#include"Log.h"

#include<spdlog/sinks/stdout_color_sinks.h>

namespace Infinity
{
	std::shared_ptr<spdlog::logger> Log::core_logger;
	std::shared_ptr<spdlog::logger> Log::client_logger;

	void Log::Init()
	{
		core_logger = spdlog::stdout_color_mt("Infinity");
		client_logger = spdlog::stdout_color_mt("Client");

		core_logger->set_level(spdlog::level::trace);
		client_logger->set_level(spdlog::level::trace);

		core_logger->set_pattern("%^[%T] %n: %v%$");
		client_logger->set_pattern("%^[%T] %n: %v%$");
	}

	std::shared_ptr<spdlog::logger> Log::GetCoreLogger() { return core_logger; }
	std::shared_ptr<spdlog::logger> Log::GetClientLogger() { return client_logger; }
}
#endif // DEBUG