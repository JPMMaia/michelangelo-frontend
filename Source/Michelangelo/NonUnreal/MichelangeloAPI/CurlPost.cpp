#include "Michelangelo.h"
#include "CurlPost.h"
#include "NonUnreal/Common/EngineException.h"

#include <sstream>

using namespace Common;
using namespace MichelangeloAPI;

void CurlPost::AddPair(const std::string& name, const std::string& value)
{
	m_pairs[name] = value;
}
void CurlPost::Generate(CURL* curl, bool urlEncode)
{
	std::stringstream stringStream;

	auto pairCount = m_pairs.size();
	for (const auto& pair : m_pairs)
	{
		stringStream << (urlEncode ? Helpers::EscapeString(curl, pair.first) : pair.first) << "=" << (urlEncode ? Helpers::EscapeString(curl, pair.second) : pair.second);
		if (--pairCount != 0)
			stringStream << "&";
	}

	// Convert to string:
	m_data = stringStream.str();
}
const std::string& CurlPost::GetData() const
{
	return m_data;
}
