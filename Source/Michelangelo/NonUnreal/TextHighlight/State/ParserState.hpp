#pragma once

#include <string>

namespace TextHighlight
{
	namespace State
	{
		class IStateObserver;

		class ParserState
		{
		public:
			explicit ParserState(IStateObserver& observer, const std::string::const_iterator& beginText) :
				m_observer(observer),
				m_beginText(beginText)
			{
			}
			virtual ~ParserState()
			{
			}

			virtual void Parse(const std::string::const_iterator& textIterator, std::shared_ptr<ParserState>& nextState) = 0;
			virtual void FoundText(const std::string::const_iterator& endText) = 0;

			const std::string::const_iterator& GetBeginText() const
			{
				return m_beginText;
			}

		protected:
			IStateObserver& m_observer;
			std::string::const_iterator m_beginText;
		};
	}
}
