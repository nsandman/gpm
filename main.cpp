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
	/* Use callback to save cURL output to variable */
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, nFunctions::curlToVar);
	char *argOne = argv[1];
	const char *iDir = string(GPMDIR + "/installed/_installed.gpi").c_str();
	if (strcmp(argOne, "install") == 0 || strcmp(argOne, "i") == 0) {
		for (int a = 2; a < argc; a++) {
			bool shouldContinue = true;
			string ca = argv[a];
			ifstream installed;
			installed.open(iDir);
			string line;
			for (int i = 0; getline(installed, line); i++) {
				if (strcmp(line.c_str(), ca.c_str()) == 0) {
					shouldContinue = false;
					break;
				}
			}
			installed.close();
			if (shouldContinue) {
				string cFile = GPMDIR + "/tmp/" + ca;
				string ghUrl = "https://raw.githubusercontent.com/nsandman09/gpm-packages/master/" + ca + ".gpm";
				/* Clear the cURL response variable */
				nFunctions::cc();
				curl_easy_setopt(curl, CURLOPT_URL, ghUrl.c_str());
				curl_easy_perform(curl);
				rapidjson::Document getUrl;
				getUrl.Parse(curlResult.c_str());
				rapidjson::Value &url = getUrl["url"];
				rapidjson::Value &cmd = getUrl["commands"];
				nFunctions::cc();
				curl_easy_setopt(curl, CURLOPT_URL, url.GetString());
				curl_easy_perform(curl);
				ofstream downloadFile;
	  			downloadFile.open(cFile.c_str());
	  			downloadFile << curlResult;
	  			downloadFile.close();
	  			for (int b = 0; b < cmd.Size(); b++) {
					string currentCommand(cmd[b].GetString());
					/* Replace the variables */
					currentCommand = nFunctions::replaceAll(currentCommand, string("{FILENAME}"), cFile);
					currentCommand = nFunctions::replaceAll(currentCommand, string("{GPMDIR}"), GPMDIR);
					system(currentCommand.c_str()); /* Execute the edited command */
				}
				/* Move from "tmp" to "installed" using nFunctions::replaceAll() in rename() */
				rename(cFile.c_str(), nFunctions::replaceAll(cFile, string("/tmp/"), string("/installed/")).c_str());
				/* Append to installed files list */
				ofstream installed;
				installed.open(iDir, ios::app);
				installed << ca << endl;
				installed.close();
			} else {
				cout << "Package \"" << ca << "\" already installed!\n";
			}
		}
		/* Clean up! */
		curl_easy_cleanup(curl);
		curl_global_cleanup();
	} 
	else if (strcmp(argOne, "list") == 0 || strcmp(argOne, "l") == 0) {
		ifstream installed;
		installed.open(iDir);
		string iOutput;
		string cLine;
		for (int i = 0; getline(installed, cLine); i++) {
			iOutput.append(cLine + "\n");
		}
		cout << "INSTALLED PACKAGES\n==================\n" << iOutput << endl;
	}
	else {
		cout << "Unknown param " << argOne << "!\n";
	}
	return 0;
}