#pragma once

#include "BaseTextPiece.hpp"
#include "NormalTextPiece.hpp"

#include <list>
#include <regex>
#include "MultiLineCommentPiece.h"
#include "OneLineCommentPiece.hpp"
#include "KeywordPiece.hpp"
#include "CSharpCodeHighlighter.hpp"

namespace TextHighlight
{
	class MixTextPiece : public BaseTextPiece
	{
	public:
		MixTextPiece() = default;
		explicit MixTextPiece(const std::string& text) :
			m_text(text)
		{
		}

		void Parse()
		{
			ParseMultiLineComments();
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			for (const auto& child : m_children)
				ss << child->ToString();
			return ss.str();
		}

	private:

		void ParseMultiLineComments()
		{
			std::regex regex("\\/\\*+((([^\\*])+)|([\\*]+(?!\\/)))[*]+\\/");

			auto expression = m_text;
			std::smatch match;
			do
			{
				
			}

			while(std::regex_search(expression, match, regex))
			{
				// Parse prefix and add it to children:
				{
					auto prefix = std::make_unique<MixTextPiece>(match.prefix().str());
					prefix->ParseOneLineComments();
					m_children.push_back(std::move(prefix));
				}

				// Add multi-line piece to children:
				m_children.push_back(std::make_unique<MultiLineCommentPiece>(match[0].str()));

				// Continue parsing the suffix:
				expression = match.suffix().str();
			}
		}

		void ParseOneLineComments()
		{
			std::regex regex("\\/\\/.*");

			auto expression = m_text;
			std::smatch match;
			while (std::regex_search(expression, match, regex))
			{
				// Parse prefix and add it to children:
				{
					auto prefix = std::make_unique<MixTextPiece>(match.prefix().str());
					prefix->ParseKeywords();
					m_children.push_back(std::move(prefix));
				}

				// Add one-line piece to children:
				m_children.push_back(std::make_unique<OneLineCommentPiece>(match[0].str()));

				// Continue parsing the suffix:
				expression = match.suffix().str();
			}
		}

		void ParseKeywords()
		{
			auto regex = CSharpCodeHighlighter::Get()->GetKeywordsRegex();;

			auto expression = m_text;
			std::smatch match;
			while (std::regex_search(expression, match, regex))
			{
				// Parse prefix and add it to children:
				{
					auto prefix = std::make_unique<MixTextPiece>(match.prefix().str());
					prefix->ParseNormals();
					m_children.push_back(std::move(prefix));
				}

				// Add keyword piece to children:
				m_children.push_back(std::make_unique<KeywordPiece>(match[0].str()));

				// Continue parsing the suffix:
				expression = match.suffix().str();
			}
		}

		void ParseNormals()
		{
			m_children.push_back(std::make_unique<NormalTextPiece>(m_text));
		}



	private:
		std::string m_text;
		std::list<std::unique_ptr<BaseTextPiece>> m_children;
	};
}
