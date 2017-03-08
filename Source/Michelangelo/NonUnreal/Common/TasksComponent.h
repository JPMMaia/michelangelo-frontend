#pragma once

#include <deque>

#include "Task.h"

namespace Common
{
	class TasksComponent
	{
	public:
		~TasksComponent();

		void Add(Task&& task);

		void Update();

		void JoinAll() noexcept;

	private:
		std::deque<Task> m_tasks;
	};
}
