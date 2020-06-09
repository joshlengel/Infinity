#pragma once

#include"Core.h"

#include<spdlog/spdlog.h>

#undef CreateWindow

#include<memory>

namespace Infinity
{
	class INFINITY_API Log
	{
	private:
		static std::shared_ptr<spdlog::logger> core_logger;
		static std::shared_ptr<spdlog::logger> client_logger;

	public:
		static void Init();
		
		static std::shared_ptr<spdlog::logger> GetCoreLogger();
		static std::shared_ptr<spdlog::logger> GetClientLogger();
	};
}