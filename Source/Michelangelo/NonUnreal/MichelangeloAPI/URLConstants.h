#pragma once

#include "GrammarType.h"

namespace MichelangeloAPI
{
	namespace URLConstants
	{
		static const std::string MainPage("https://michelangelo.graphics/");
		static const std::string LogInAPI("https://michelangelo.graphics/Account/Login/");
		static const std::string LogOutAPI("https://michelangelo.graphics/Account/LogOff");
		static const std::string OwnGrammarAPI("https://michelangelo.graphics/api/Grammar/");
		static const std::string SharedGrammarAPI("https://michelangelo.graphics/api/Shared/0/");
		static const std::string TutorialAPI("https://michelangelo.graphics/api/Tutorial/");

		static const std::map<GrammarType, const std::string&> GetGrammarsListURL = 
		{
			{ GrammarType::Own, OwnGrammarAPI },
			{ GrammarType::Shared, SharedGrammarAPI },
			{ GrammarType::Tutorial, TutorialAPI },
		};

		static const std::map<GrammarType, const std::string&> GetGrammarDataURL =
		{
			{ GrammarType::Own, OwnGrammarAPI },
			{ GrammarType::Shared, OwnGrammarAPI },
			{ GrammarType::Tutorial, TutorialAPI },
		};

		static const std::map<GrammarType, const std::string&> EvaluateGrammarURL =
		{
			{ GrammarType::Own, OwnGrammarAPI },
			{ GrammarType::Shared, OwnGrammarAPI },
			{ GrammarType::Tutorial, TutorialAPI },
		};
	}
}
