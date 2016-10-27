#pragma once

#pragma comment(lib, "pdh.lib")

#include <cstdint>
#include <pdh.h>

namespace Common
{
	class Timer
	{
	public:
		explicit Timer(double millisecondsPerUpdate);

		void Reset();

		template <typename UpdateFunctionType, typename RenderFunctionType, typename ProcessInputFunctionType, typename ProcessFrameStatisticsFunctionType>
		bool UpdateAndRender(UpdateFunctionType&& update, RenderFunctionType&& render, ProcessInputFunctionType&& processInput, ProcessFrameStatisticsFunctionType&& processFrameStatistics);

		double GetMillisecondsPerUpdate() const;
		double GetTotalMilliseconds() const;
		double GetDeltaMilliseconds() const;
		double GetLeftover() const;
		size_t GetFramesPerSecond() const;
		double GetMillisecondsPerFrame() const;

	private:
		double TicksToMilliseconds(uint64_t ticks) const;
		static LARGE_INTEGER GetCurrentTick();

		template<typename ProcessFrameStatisticsFunctionType>
		void CalculateFrameStatistics(ProcessFrameStatisticsFunctionType&& processFrameStatistics);
		
	private:
		const double m_millisecondsPerUpdate;

		uint64_t m_totalTicks;
		uint64_t m_deltaTicks;
		double m_lag;

		LARGE_INTEGER m_qpcFrequency;
		LARGE_INTEGER m_qpcPreviousTick;
		LARGE_INTEGER m_qpcCurrentTick;

		size_t m_framesPerSecond = 0;
		double m_millisecondsPerFrame = 0.0f;
	};

	template <typename UpdateFunctionType, typename RenderFunctionType, typename ProcessInputFunctionType, typename ProcessFrameStatisticsFunctionType>
	bool Timer::UpdateAndRender(UpdateFunctionType&& update, RenderFunctionType&& render, ProcessInputFunctionType&& processInput, ProcessFrameStatisticsFunctionType&& processFrameStatistics)
	{
		m_qpcCurrentTick = GetCurrentTick();
		m_deltaTicks = m_qpcCurrentTick.QuadPart - m_qpcPreviousTick.QuadPart;
		m_qpcPreviousTick = m_qpcCurrentTick;
		m_totalTicks += m_deltaTicks;

		auto deltaMilliseconds = TicksToMilliseconds(m_deltaTicks);
		m_lag += deltaMilliseconds;

		// Process input:
		if(!processInput())
			return false;

		// Update the times needed to catchup:
		while (m_lag >= m_millisecondsPerUpdate)
		{
			update(*this);
			m_lag -= m_millisecondsPerUpdate;
		}

		// Render:
		render(*this);

		// Calculate frames statistics (frames per second, milliseconds per frame):
		CalculateFrameStatistics(processFrameStatistics);

		return true;
	}

	template <typename ProcessFrameStatisticsFunctionType>
	void Timer::CalculateFrameStatistics(ProcessFrameStatisticsFunctionType&& processFrameStatistics)
	{
		static auto elapsedMilliseconds = 0.0;
		static size_t frameCount = 0;

		// Increment frame count:
		++frameCount;

		// If one second has elapsed:
		if (GetTotalMilliseconds() - elapsedMilliseconds >= 1000.0)
		{
			// Update values:
			m_framesPerSecond = frameCount;
			m_millisecondsPerFrame = 1000.0 / static_cast<double>(frameCount);

			// Reset frame count and increment elapsed time by 1 second:
			frameCount = 0;
			elapsedMilliseconds += 1000.0;

			processFrameStatistics(*this);
		}
	}
}
