#pragma once

#include"Core.h"

#include<spdlog/spdlog.h>

#undef CreateWindow

#include"utils/data/Pointer.h"

#include <memory>

namespace std { INFINITY_TEMPLATE template class INFINITY_API shared_ptr<spdlog::logger>; }

namespace Infinity
{
	INFINITY_TEMPLATE template class INFINITY_API Pointer<spdlog::logger>;

	class INFINITY_API Log
	{
	private:
		static std::shared_ptr<spdlog::logger> sp_core_logger;
		static std::shared_ptr<spdlog::logger> sp_client_logger;

		static Pointer<spdlog::logger> core_logger;
		static Pointer<spdlog::logger> client_logger;

	public:
		static void Init();
		
		static Pointer<spdlog::logger> GetCoreLogger();
		static Pointer<spdlog::logger> GetClientLogger();
	};
}