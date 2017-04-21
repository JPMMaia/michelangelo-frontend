#pragma once

#include "ParserState.hpp"
#include "Normal.h"

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
					nextState = std::make_shared<State::Normal>(m_observer, textIterator);
					FoundText(textIterator);
				}
			}
			void FoundText(const std::string::const_iterator& endText) override
			{
				m_observer.FoundOneLineComment(m_beginText, endText);
			}
		};
	}
}
