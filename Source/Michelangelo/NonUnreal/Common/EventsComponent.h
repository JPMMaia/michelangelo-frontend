#pragma once

#include <deque>
#include <mutex>

#include "Event.h"

namespace Common
{
	template<typename SenderType>
	class EventsComponent
	{
	public:
		void HandleEvents(SenderType& sender)
		{
			std::lock_guard<std::mutex> lock(m_eventsMutex);

			while(!m_events.empty())
			{
				m_events.front()->Handle(sender);
				m_events.pop_front();
			}
		}

		void AddEvent(std::unique_ptr<Event<SenderType>>&& event)
		{
			std::lock_guard<std::mutex> lock(m_eventsMutex);
			m_events.emplace_back(std::move(event));
		}

	private:
		std::mutex m_eventsMutex;
		std::deque<std::unique_ptr<Event<SenderType>>> m_events;
	};
}
