/*
        Project
 ––––––––––––––––––––––
|   ____ ____  __  __  |
|  / ___|  _ \|  \/  | |
| | |  _| |_) | |\/| | |
| | |_| |  __/| |  | | |
|  \____|_|   |_|  |_| |
|                      |
|    by Noah Sandman   |
 ––––––––––––––––––––––
*/

#include <pwd.h>
#include <fstream>
#include <iostream>
#include "curl/curl.h"
#include "rapidjson/document.h"

std::string pkgURL;
std::string currentArg;
std::string returnedText;
std::string gpmdir = getenv("HOME");

/* By "heikaman" from http://ubuntuforums.org/archive/index.php/t-1435926.html */
size_t curl_write(void *ptr, size_t size, size_t nmemb, void *stream) {
	returnedText.append((char*)ptr);
	return size*nmemb;
}

int main(int argc, const char *argv[]) {
	gpmdir.append("/.gpm");
	CURL *curl = curl_easy_init();
	std::string arg = argv[0];
	std::string initialArg = argv[1];
	if (initialArg == "install" || initialArg == "i") {
		for (int a = 2; a < argc; a++) {
			currentArg = argv[a];
			std::cout << "\033[00;32mInstalling package \"" << currentArg << "\"...\033[0m\n";
			pkgURL = "https://raw.githubusercontent.com/nsandman09/gpm-packages/master/" + currentArg + ".gpm";
			curl_easy_setopt(curl, CURLOPT_URL, pkgURL.c_str()); 
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write);
			curl_easy_perform(curl);
			if (returnedText == "Not Found") {
				std::cout << "\033[00;31mThe package \"" << currentArg << "\" was not found.\033[0m\n";
			} else {
				rapidjson::Document file;
				file.Parse(returnedText.c_str());
				rapidjson::Value &url = file["url"];
				//std::cout << url.GetString();
				returnedText = "";
				curl_easy_setopt(curl, CURLOPT_URL, url.GetString()); 
				curl_easy_perform(curl);
				std::ofstream dl;
				dl.open(gpmdir + "/tmp/" + currentArg);
				dl << returnedText;
			}
			returnedText = "";
		}
	} else {
		std::cout << "\033[00;31mCommand \033[1;31m" + arg + " " + initialArg + " \033[0m\033[00;31mnot found.\033[0m\n";
	}
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	return 0;
}