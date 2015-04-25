/* main.cpp - Noah Sandman
   04/24/2014 */

#include <cstring>
#include <fstream>
#include <iostream>
#include <curl/curl.h>

#include "main.h"
#include "rapidjson/document.h"
using namespace std;

int main(int argc, const char *argv[]) {
	CURL *curl = curl_easy_init();
	const char *argOne = argv[1];
	if (strcmp(argOne, "install") == 0 || strcmp(argOne, "i") == 0) {
		for (int a = 2; a < argc; a++) {
			string ca = argv[a];
			string ghUrl = "https://raw.githubusercontent.com/nsandman09/gpm-packages/master/" + ca + ".gpm";
			cc();
			curl_easy_setopt(curl, CURLOPT_URL, ghUrl.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlToVar);
			curl_easy_perform(curl);
			rapidjson::Document getUrl;
			getUrl.Parse(curlResult.c_str());
			rapidjson::Value &url = getUrl["url"];
			cc();
			curl_easy_setopt(curl, CURLOPT_URL, url.GetString());
			curl_easy_perform(curl);
			cout << curlResult << endl;
		}
		curl_easy_cleanup(curl);
		curl_global_cleanup();
	} else {
		cout << "Unknown param " << argOne << "!\n";
	}
	return 0;
}