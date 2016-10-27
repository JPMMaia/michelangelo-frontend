#include "Timer.h"
#include "EngineException.h"

using namespace Common;

Timer::Timer(double millisecondsPerUpdate) :
	m_millisecondsPerUpdate(millisecondsPerUpdate),
	m_totalTicks(0),
	m_deltaTicks(0),
	m_lag(0)
{
	if (!QueryPerformanceFrequency(&m_qpcFrequency))
	{
		ThrowEngineException(L"QueryPerformanceFrequency failed!");
	}
}

void Timer::Reset()
{
	m_qpcPreviousTick = GetCurrentTick();
	m_lag = 0.0;
}

double Timer::GetMillisecondsPerUpdate() const
{
	return m_millisecondsPerUpdate;
}
double Timer::GetTotalMilliseconds() const
{
	return TicksToMilliseconds(m_totalTicks);
}
double Timer::GetDeltaMilliseconds() const
{
	return TicksToMilliseconds(m_deltaTicks);
}
double Timer::GetLeftover() const
{
	return m_lag / m_millisecondsPerUpdate;
}
size_t Timer::GetFramesPerSecond() const
{
	return m_framesPerSecond;
}
double Timer::GetMillisecondsPerFrame() const
{
	return m_millisecondsPerFrame;
}

LARGE_INTEGER Timer::GetCurrentTick()
{
	LARGE_INTEGER currentTime;
	if (!QueryPerformanceCounter(&currentTime))
	{
		ThrowEngineException(L"QueryPerformanceCounter failed!");
	}

	return currentTime;
}

double Timer::TicksToMilliseconds(uint64_t ticks) const
{
	return static_cast<double>((ticks * 1000)) / static_cast<double>(m_qpcFrequency.QuadPart);
}
