#if defined(_DEBUG)
#pragma comment(lib, "libcurl_a_debug.lib")
#else
#pragma comment(lib, "libcurl_a.lib")
#endif


#include "WebAPI.h"

int main()
{
	using namespace MichelangeloAPI;	

	WebAPI webAPI;
	webAPI.Authenticate();
	auto tutorials = webAPI.GetTutorials();

	return 0;
}