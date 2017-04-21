#pragma once
#include <string>

namespace TextHighlight
{
	namespace State
	{
		class IStateObserver
		{
		public:
			virtual ~IStateObserver()
			{
			}

		public:
			virtual void FoundNormalText(const std::string::const_iterator& begin, const std::string::const_iterator& end) = 0;
			virtual void FoundString(const std::string::const_iterator& begin, const std::string::const_iterator& end) = 0;
			virtual void FoundOneLineComment(const std::string::const_iterator& begin, const std::string::const_iterator& end) = 0;
			virtual void FoundMultiLineComment(const std::string::const_iterator& begin, const std::string::const_iterator& end) = 0;
		};
	}
}
