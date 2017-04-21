#pragma once

namespace Common
{
	template<typename SenderType>
	class Event
	{
	public:
		virtual ~Event() = default;

		virtual void Handle(SenderType& sender) = 0;
	};
}