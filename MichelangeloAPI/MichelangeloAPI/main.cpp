// UnrealConnectionTest.cpp : Defines the entry point for the console application.
//
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <regex>
#include <iostream>

#if defined(_DEBUG)
#pragma comment(lib, "libcurl_a_debug.lib")
#else
#pragma comment(lib, "libcurl_a.lib")
#endif

static int writer(char *data, size_t size, size_t nmemb, std::string *writerData)
{
	if (writerData == NULL)
		return 0;

	writerData->append(data, size*nmemb);
	return size * nmemb;
}

static std::string requestTokenCookie;
static std::string buffer1;
static std::string buffer2;
static std::string buffer3;
static std::string buffer4;
static int rounds = 0;
static const std::string str_setCookie("Set-Cookie:");
static const std::string str_requestCookie("__Request");
static const std::string str_applicationCookie(".AspNet.ApplicationCookie");
static const std::string str_semicolon("; ");

static size_t header_callback(char *buffer, size_t size, size_t nitems, void *userdata)
{
	if (rounds < 2) //trigger only if the tokens have not been extracted yet (NOTE: It may happen that the tokens get refreshed, e.g. when the server restarts. It would be fine to detect that and update them. In that case the rounds counter should be set back to 0.)
	{
		std::string str(buffer); //convert the char* to string

		if (str.find(str_setCookie) != std::string::npos) //is there a setcookie in the response?
		{
			//first request round should yield the requestVerification Cookie
			if (rounds == 0)
			{
				int start = str.find(str_requestCookie);
				if (start >= 0)
				{
					int end = str.find(str_semicolon, start);
					requestTokenCookie = "Cookie:" + str.substr(start, end - start);
					++rounds;
				}
			}
			else
			{
				//second request round should yield the aplication cookie
				int start = str.find(str_applicationCookie);
				if (start >= 0)
				{
					int end = str.find(str_semicolon, start);
					requestTokenCookie = requestTokenCookie + str_semicolon + str.substr(start, end - start);
					++rounds;
				}
			}
		}
	}
	return nitems * size;
}

int main()
{
	CURL *curl;
	CURLcode res;

	// In windows, this will init the winsock stuff
	res = curl_global_init(CURL_GLOBAL_DEFAULT);
	if (res != CURLE_OK) {
		fprintf(stderr, "curl_global_init() failed: %s\n",
			curl_easy_strerror(res));
		return 1;
	}

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "https://michelangelo.graphics/Account/Login"); //TODO in case of timeout retry
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L); //no progress meter please

														//capture the response body into a buffer
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer1);
		//process the response headers using a callback funciton
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
			fprintf(stderr, "\n\ncurl_easy_perform() failed: %s\n", curl_easy_strerror(res));

		//We have obtained the login screen here, we need to extract the request verification token
		//TODO now it works only for direct registrations on the site, it would be good to extend it to external logins (Google, Facebook) if possible. They have a different token and possibly the login sequence is different as well.
		std::regex token_regex("form action=\"\\/Account\\/Login\" .*><input name=\"__RequestVerificationToken\" type=\"hidden\" value=\".*\"");
		std::smatch match;
		std::string tag;
		std::regex_search(buffer1, match, token_regex);
		for (auto x : match) tag = x;
		//now there should be quite a large substring in the tag including the form and its first input field (which bears the token)
		std::regex tokenValue_regex("value=\".*\""); //match the value of the token
		std::regex_search(tag, match, tokenValue_regex);
		for (auto x : match) tag = x;
		std::string requestVerificationToken = tag.substr(7, tag.length() - 8); //remove the value=" at the begining and the " at the end


																				//Put the cookie token into the header (attention, the request token of the cookie is a different one than the request token of the login form body)
		struct curl_slist *chunk = NULL;
		chunk = curl_slist_append(chunk, requestTokenCookie.c_str());
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

		//The credentials go here, plus the body request token
		//!!!
		//TODO replace the LOGIN and PWD by the username and password of the user!!! Otherwise it wil not work!
		//!!!
		std::string postBody = "__RequestVerificationToken=" + requestVerificationToken + "&Email=jpmmaia@gmail.com&Password=yslxqCIVAIqYFuAqYUImyNo5375NYhVGyVwdNerkrjnV8HMEPnwiyQBISnSAThj5&RememberMe=false";
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postBody.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)postBody.length());
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer2);

		//Now we will authorize the user and receive back an application token
		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
			fprintf(stderr, "\n\ncurl_easy_perform() failed: %s\n", curl_easy_strerror(res));

		//free the request header with the cookie token
		curl_slist_free_all(chunk);
		//The application token has ben extracted from the response header and added to the cookie. Let us set the updated cookie for the request header.
		chunk = NULL;
		chunk = curl_slist_append(chunk, requestTokenCookie.c_str());
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

		//TODO this can be probably omited ... only for testing here. It gets the main page html code.
		curl_easy_setopt(curl, CURLOPT_URL, "https://michelangelo.graphics");
		curl_easy_setopt(curl, CURLOPT_POST, 0L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 0L);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer3);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
			fprintf(stderr, "\n\ncurl_easy_perform() failed: %s\n", curl_easy_strerror(res));

		//This is the interesting part! Here we request the list of tutorials and obtain a JSON object with all tutorials grammars.
		//The same can be done for endpoints: /api/Grammar and /api/Shared/0/
		curl_easy_setopt(curl, CURLOPT_URL, "https://michelangelo.graphics/api/Tutorial/");
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer4);

		//TODO: To find out about further endpoints, I suggest to use the network monitor in either Firefox or Chrome (with the persisten log option enabled) to see what other requests are performed to GET a single grammmar and to POST it back to the server.

		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
			fprintf(stderr, "\n\ncurl_easy_perform() failed: %s\n", curl_easy_strerror(res));

		//clean up the header
		curl_slist_free_all(chunk);
		//cleanup the curl
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();

	return 0;
}