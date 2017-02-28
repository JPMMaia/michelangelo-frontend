#pragma once

#include "BaseTextPiece.hpp"
#include "NormalTextPiece.hpp"

#include <list>
#include <regex>
#include "MultiLineCommentPiece.h"
#include "OneLineCommentPiece.hpp"
#include "KeywordPiece.hpp"
#include "CSharpHighlightSettings.hpp"
#include "State/Normal.hpp"
#include "StringPiece.h"

namespace TextHighlight
{
	class CSharpHighlighter : public BaseTextPiece, public State::IStateObserver
	{
	public:
		CSharpHighlighter() = default;
		explicit CSharpHighlighter(const std::string& text) :
			m_text(text)
		{
		}

		void Parse()
		{
			auto iterator = m_text.cbegin();
			std::shared_ptr<State::ParserState> currentState(std::make_shared<State::Normal>(*this, iterator));
			while (iterator != m_text.cend())
			{
				auto nextState = currentState;
				currentState->Parse(iterator, nextState);
				currentState = nextState;
				++iterator;
			}
			currentState->FoundText(iterator);
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			for (const auto& child : m_children)
				ss << child->ToString();
			return ss.str();
		}

		void FoundNormalText(const std::string::const_iterator& begin, const std::string::const_iterator& end) override
		{
			if (begin != end)
				ParseNormalText(std::string(begin, end));
		}
		void FoundString(const std::string::const_iterator& begin, const std::string::const_iterator& end) override
		{
			if (begin != end)
				m_children.push_back(std::make_unique<StringPiece>(std::string(begin, end)));
		}
		void FoundOneLineComment(const std::string::const_iterator& begin, const std::string::const_iterator& end) override
		{
			if (begin != end)
				m_children.push_back(std::make_unique<OneLineCommentPiece>(std::string(begin, end)));
		}
		void FoundMultiLineComment(const std::string::const_iterator& begin, const std::string::const_iterator& end) override
		{
			if (begin != end)
				m_children.push_back(std::make_unique<MultiLineCommentPiece>(std::string(begin, end)));
		}

	private:

		void ParseNormalText(const std::string& text)
		{
			auto regex = CSharpHighlightSettings::Get()->GetKeywordsRegex();;

			auto expression = text;
			std::smatch match;
			while (std::regex_search(expression, match, regex))
			{
				// Add prefix as normal text:
				{
					auto prefix = match.prefix().str();
					if(!prefix.empty())
						m_children.push_back(std::make_unique<NormalTextPiece>(match.prefix().str()));
				}

				// Add keyword piece to children:
				m_children.push_back(std::make_unique<KeywordPiece>(match[0].str()));

				// Continue parsing the suffix:
				expression = match.suffix().str();
			}

			if(!expression.empty())
				m_children.push_back(std::make_unique<NormalTextPiece>(expression));
		}

	private:
		std::string m_text;
		std::list<std::unique_ptr<BaseTextPiece>> m_children;
	};
}
