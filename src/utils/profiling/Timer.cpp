#include"InfinityPCH.h"

#include"utils/profiling/Timer.h"

namespace Infinity
{
	Timer::Timer(const char *name):
		m_name(name),
		m_stopped(false),
		m_start(std::chrono::high_resolution_clock::now())
	{}

	Timer::~Timer()
	{
		if (!m_stopped) Stop();
	}

	void Timer::Stop()
	{
		std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();

		double ms = std::chrono::duration_cast<std::chrono::microseconds>(stop - m_start).count() * 0.001;

		INFINITY_CORE_INFO("Timer '{0}': {1} ms", m_name, ms);

		m_stopped = true;
	}
}