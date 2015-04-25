/* main.cpp - Noah Sandman
   04/24/2015 */

#include <cstring>
#include <fstream>
#include <iostream>
#include <curl/curl.h>

#include "main.h"
#include "rapidjson/document.h"
using namespace std;

int main(int argc, char *argv[]) {
	string GPMDIR = strcat(getenv("HOME"), "/.gpm");
	CURL *curl = curl_easy_init();
	char *argOne = argv[1];
	if (strcmp(argOne, "install") == 0 || strcmp(argOne, "i") == 0) {
		for (int a = 2; a < argc; a++) {
			string ca = argv[a];
			string cFile = GPMDIR + "/tmp/" + ca;
			string ghUrl = "https://raw.githubusercontent.com/nsandman09/gpm-packages/master/" + ca + ".gpm";
			cc();
			curl_easy_setopt(curl, CURLOPT_URL, ghUrl.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlToVar);
			curl_easy_perform(curl);
			rapidjson::Document getUrl;
			getUrl.Parse(curlResult.c_str());
			rapidjson::Value &url = getUrl["url"];
			rapidjson::Value &cmd = getUrl["commands"];
			cc();
			curl_easy_setopt(curl, CURLOPT_URL, url.GetString());
			curl_easy_perform(curl);
			ofstream downloadFile;
  			downloadFile.open(cFile);
  			downloadFile << curlResult;
  			downloadFile.close();
  			for (int b = 0; b < cmd.Size(); b++) {
				string currentCommand(cmd[b].GetString());
				currentCommand = replaceAll(currentCommand, string("{FILENAME}"), cFile);
				currentCommand = replaceAll(currentCommand, string("{GPMDIR}"), GPMDIR);
				cout << currentCommand << endl;
				system(currentCommand.c_str());
			}
		}
		curl_easy_cleanup(curl);
		curl_global_cleanup();
	} else {
		cout << "Unknown param " << argOne << "!\n";
	}
	return 0;
}