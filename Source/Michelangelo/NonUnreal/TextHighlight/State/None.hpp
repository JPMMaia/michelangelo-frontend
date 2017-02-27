#pragma once

#include "None.h"
#include "ParserState.hpp"
#include "InsideString.hpp"
#include "OneLineComment.hpp"
#include "MultiLineComment.hpp"
#include "IStateObserver.h"

namespace TextHighlight
{
	namespace State
	{
		inline None::None(IStateObserver& observer, const std::string::const_iterator& beginText) :
			ParserState(observer, beginText),
			m_thisState(ThisState::None)
		{
		}

		inline void None::Parse(const std::string::const_iterator& textIterator, std::shared_ptr<ParserState>& nextState)
		{
			if(m_thisState == ThisState::None)
			{
				if (*textIterator == '"')
				{
					nextState = std::make_shared<State::InsideString>(m_observer, textIterator);
					m_observer.FoundNormalText(m_beginText, textIterator);
				}
				else if (*textIterator == '/')
				{
					m_thisState = ThisState::FirstSlash;
				}
			}
			else if(m_thisState == ThisState::FirstSlash)
			{
				if (*textIterator == '/')
				{
					nextState = std::make_shared<State::OneLineComment>(m_observer, textIterator - 1);
					m_observer.FoundNormalText(m_beginText, textIterator - 1);
				}
				else if (*textIterator == '*')
				{
					nextState = std::make_shared<State::MultiLineComment>(m_observer, textIterator - 1);
					m_observer.FoundNormalText(m_beginText, textIterator - 1);
				}
				else
				{
					m_thisState = ThisState::None;
				}
			}
		}
		inline void None::FoundText(const std::string::const_iterator& endText)
		{
			m_observer.FoundNormalText(m_beginText, endText);
		}

	}
}
