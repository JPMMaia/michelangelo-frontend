#pragma once

#include "ParserState.hpp"
#include "Normal.h"

namespace TextHighlight
{
	namespace State
	{
		class InsideString : public ParserState
		{
		public:
			explicit InsideString(IStateObserver& observer, const std::string::const_iterator& beginText) :
				ParserState(observer, beginText),
				m_thisState(ThisState::None)
			{
			}

			void Parse(const std::string::const_iterator& textIterator, std::shared_ptr<ParserState>& nextState) override
			{
				if(m_thisState == ThisState::None)
				{
					if(*textIterator == '"')
					{
						nextState = std::make_shared<State::Normal>(m_observer, textIterator + 1);
						m_observer.FoundString(m_beginText, textIterator + 1);
					}
					else if(*textIterator == '\n')
					{
						nextState = std::make_shared<State::Normal>(m_observer, textIterator);
						m_observer.FoundString(m_beginText, textIterator);
					}
					else if(*textIterator == '\\')
					{
						m_thisState = ThisState::Escape;
					}
				}
				else if(m_thisState == ThisState::Escape)
				{
					m_thisState = ThisState::None;
				}
			}
			void FoundText(const std::string::const_iterator& endText) override
			{
				m_observer.FoundString(m_beginText, endText);
			}

		private:
			enum class ThisState
			{
				None,
				Escape
			};

		private:
			ThisState m_thisState;
		};
	}
}
