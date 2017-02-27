#pragma once

#include "ParserState.hpp"
#include "None.h"

namespace TextHighlight
{
	namespace State
	{
		class MultiLineComment : public ParserState
		{
		public:
			explicit MultiLineComment(IStateObserver& observer, const std::string::const_iterator& beginText) :
				ParserState(observer, beginText),
				m_thisState(ThisState::None)
			{
			}

			void Parse(const std::string::const_iterator& textIterator, std::shared_ptr<ParserState>& nextState) override
			{
				if(m_thisState == ThisState::None)
				{
					if (*textIterator == '*')
					{
						m_thisState = ThisState::Asterisk;
					}
				}
				else if (m_thisState == ThisState::Asterisk)
				{
					if(*textIterator == '/')
					{
						nextState = std::make_shared<State::None>(m_observer, textIterator + 1);
						m_observer.FoundMultiLineComment(m_beginText, textIterator + 1);
					}
					else if(*textIterator != '*')
					{
						m_thisState = ThisState::None;
					}
				}
			}
			void FoundText(const std::string::const_iterator& endText) override
			{
				m_observer.FoundMultiLineComment(m_beginText, endText);
			}

		private:
			enum class ThisState
			{
				None,
				Asterisk
			};

		private:
			ThisState m_thisState;
		};
	}
}
