#pragma once

#include "ParserState.hpp"
#include "None.h"

namespace TextHighlight
{
	namespace State
	{
		class OneLineComment : public ParserState
		{
		public:
			explicit OneLineComment(IStateObserver& observer, const std::string::const_iterator& beginText) :
				ParserState(observer, beginText)
			{
			}

			void Parse(const std::string::const_iterator& textIterator, std::shared_ptr<ParserState>& nextState) override
			{
				if (*textIterator == '\n')
				{
					nextState = std::make_shared<State::None>(m_observer, textIterator + 1);
					FoundText(textIterator + 1);
				}
			}
			void FoundText(const std::string::const_iterator& endText) override
			{
				m_observer.FoundOneLineComment(m_beginText, endText);
			}
		};
	}
}
