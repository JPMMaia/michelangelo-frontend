#pragma once

#include "ParserState.hpp"
#include "IStateObserver.h"

#include <memory>

namespace TextHighlight
{
	namespace State
	{
		class None : public ParserState
		{
		public:
			explicit None(IStateObserver& observer, const std::string::const_iterator& beginText);

			void Parse(const std::string::const_iterator& textIterator, std::shared_ptr<ParserState>& nextState) override;
			void FoundText(const std::string::const_iterator& endText) override;

		private:
			enum class ThisState
			{
				None,
				FirstSlash
			};

		private:
			ThisState m_thisState;
		};
	}
}
