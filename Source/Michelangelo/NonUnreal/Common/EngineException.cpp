#include "Michelangelo.h"
#include "EngineException.h"

#include <sstream>

using namespace Common;
using namespace std;


EngineException::EngineException(const std::wstring& message, const std::wstring& functionName, const std::wstring& filename, int lineNumber)
{
	stringstream ss;

	ss << L"Message: " << Helpers::WStringToString(message) << endl;
	ss << L"Function Name: " << Helpers::WStringToString(functionName) << endl;
	ss << L"Filename: " << Helpers::WStringToString(filename) << endl;
	ss << L"Line Number: " << lineNumber << endl;

	m_message = ss.str();
}

char const* EngineException::what() const
{
	return m_message.c_str();
}
