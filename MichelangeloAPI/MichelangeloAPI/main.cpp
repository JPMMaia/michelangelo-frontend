// UnrealConnectionTest.cpp : Defines the entry point for the console application.
//
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <regex>
#include <iostream>
#include "WebAPI.h"

#if defined(_DEBUG)
#pragma comment(lib, "libcurl_a_debug.lib")
#else
#pragma comment(lib, "libcurl_a.lib")
#endif

int main()
{
	using namespace MichelangeloAPI;

	CURLcode res;

	WebAPI webAPI;
	webAPI.Authenticate();

	auto curl = webAPI.GetCURL();

	// The application token has been extracted from the response header and added to the cookie. Let us set the updated cookie for the request header.
	ThrowIfCURLFailed(curl_easy_setopt(curl, CURLOPT_HTTPHEADER, webAPI.GetCookie()));

	

	//TODO this can be probably omited ... only for testing here. It gets the main page html code.
	curl_easy_setopt(curl, CURLOPT_URL, "https://michelangelo.graphics");
	curl_easy_setopt(curl, CURLOPT_POST, 0L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 0L);

	std::string buffer3;
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer3);

	res = curl_easy_perform(curl);
	if (res != CURLE_OK)
		fprintf(stderr, "\n\ncurl_easy_perform() failed: %s\n", curl_easy_strerror(res));

	//This is the interesting part! Here we request the list of tutorials and obtain a JSON object with all tutorials grammars.
	//The same can be done for endpoints: /api/Grammar and /api/Shared/0/
	curl_easy_setopt(curl, CURLOPT_URL, "https://michelangelo.graphics/api/Tutorial/");

	std::string buffer4;
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer4);

	//TODO: To find out about further endpoints, I suggest to use the network monitor in either Firefox or Chrome (with the persisten log option enabled) to see what other requests are performed to GET a single grammmar and to POST it back to the server.

	res = curl_easy_perform(curl);
	if (res != CURLE_OK)
		fprintf(stderr, "\n\ncurl_easy_perform() failed: %s\n", curl_easy_strerror(res));

	return 0;
}