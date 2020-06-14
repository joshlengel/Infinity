#pragma once

#ifdef DEBUG

#include"Core.h"

#include<spdlog/spdlog.h>

#undef CreateWindow

#include <memory>

namespace std { INFINITY_TEMPLATE template class INFINITY_API shared_ptr<spdlog::logger>; }

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

#endif // DEBUG