#pragma once

#include"Core.h"

#include<chrono>

namespace std::chrono
{
	INFINITY_TEMPLATE template class INFINITY_API duration<__int64, std::nano>;
	INFINITY_TEMPLATE template class INFINITY_API time_point<steady_clock, nanoseconds>;
}

namespace Infinity
{
	class INFINITY_API Timer
	{
	private:
		const char *m_name;
		bool m_stopped;
		std::chrono::high_resolution_clock::time_point m_start;

	public:
		Timer(const char *name);
		~Timer();

		void Stop();
	};
}