#pragma once

#ifdef INFINITY_WINDOWS
	#ifdef INFINITY_BUILD_DLL
		#define INFINITY_API __declspec(dllexport)
		#define INFINITY_TEMPLATE
	#else
		#define INFINITY_API __declspec(dllimport)
		#define INFINITY_TEMPLATE extern
	#endif // INFINITY_BUILD_DLL
#else
	#error Infinity game engine only supports windows currently
#endif // INFINITY_WINDOWS

#define INFINITY_DONT_CARE 0xffffffff

#ifdef DEBUG
	#ifdef INFINITY_BUILD_DLL
		#define INFINITY_CORE_TRACE(msg, ...) Log::GetCoreLogger()->trace(msg, __VA_ARGS__)
		#define INFINITY_CORE_INFO(msg, ...)  Log::GetCoreLogger()->info(msg, __VA_ARGS__)
		#define INFINITY_CORE_WARN(msg, ...)  Log::GetCoreLogger()->warn(msg, __VA_ARGS__)
		#define INFINITY_CORE_ERROR(msg, ...) Log::GetCoreLogger()->error(msg, __VA_ARGS__)
		#define INFINITY_CORE_FATAL(msg, ...) Log::GetCoreLogger()->critical(msg, __VA_ARGS__)
	#endif // INFINITY_BUILD_DLL

	#define INFINITY_CLIENT_TRACE(msg, ...) Infinity::Log::GetClientLogger()->trace(msg, __VA_ARGS__)
	#define INFINITY_CLIENT_INFO(msg, ...)  Infinity::Log::GetClientLogger()->info(msg, __VA_ARGS__)
	#define INFINITY_CLIENT_WARN(msg, ...)  Infinity::Log::GetClientLogger()->warn(msg, __VA_ARGS__)
	#define INFINITY_CLIENT_ERROR(msg, ...) Infinity::Log::GetClientLogger()->error(msg, __VA_ARGS__)
	#define INFINITY_CLIENT_FATAL(msg, ...) Infinity::Log::GetClientLogger()->critical(msg, __VA_ARGS__)

	#define INFINITY_PROFILE_NAME2(name, line) name##line
	#define INFINITY_PROFILE_NAME1(name, line) INFINITY_PROFILE_NAME2(name, line)
	#define INFINITY_PROFILE_SCOPE(name) Infinity::Timer INFINITY_PROFILE_NAME1(timer, __LINE__) (name)
#else
	#ifdef INFINITY_BUILD_DLL
		#define INFINITY_CORE_TRACE(msg, ...)
		#define INFINITY_CORE_INFO(msg, ...)
		#define INFINITY_CORE_WARN(msg, ...)
		#define INFINITY_CORE_ERROR(msg, ...)
		#define INFINITY_CORE_FATAL(msg, ...)
	#endif // INFINITY_BUILD_DLL

	#define INFINITY_CLIENT_TRACE(msg, ...)
	#define INFINITY_CLIENT_INFO(msg, ...)
	#define INFINITY_CLIENT_WARN(msg, ...)
	#define INFINITY_CLIENT_ERROR(msg, ...)
	#define INFINITY_CLIENT_FATAL(msg, ...)

	#define INFINITY_PROFILE_SCOPE(name)
#endif // DEBUG