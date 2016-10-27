#include "EngineException.h"

#include <sstream>

using namespace Common;
using namespace std;

EngineException::EngineException(const std::wstring& message, const std::wstring& functionName, const std::wstring& filename, int lineNumber) :
	m_message(message),
	m_functionName(functionName),
	m_filename(filename),
	m_lineNumber(lineNumber)
{
}

wstring EngineException::ToString() const
{
	wstringstream ss;

	ss << L"Message: " << m_message << endl;
	ss << L"Function Name: " << m_functionName << endl;
	ss << L"Filename: " << m_filename << endl;
	ss << L"Line Number: " << m_lineNumber << endl;

	return ss.str();
}
