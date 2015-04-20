#include <pwd.h>
#include <fstream>
#include <iostream>
#include "curl/curl.h"
#include "rapidjson/document.h"

using namespace std;

string pkgURL;
string currentArg;
string returnedText;
string currentCommand;
string gpmdir = getenv("HOME");

/* By "heikaman" from http://ubuntuforums.org/archive/index.php/t-1435926.html */
size_t curl_write(void *ptr, size_t size, size_t nmemb, void *stream) {
	returnedText.append((char*)ptr, size*nmemb);
	return size*nmemb;
}

/* By James Kanze from http://stackoverflow.com/questions/20406744 */
string replaceAll(string const& original, string const& from, string const& to) {
    string results;
    string::const_iterator end = original.end();
    string::const_iterator current = original.begin();
    string::const_iterator next = search(current, end, from.begin(), from.end());
    while (next != end) {
        results.append(current, next);
        results.append(to);
        current = next + from.size();
        next = search(current, end, from.begin(), from.end());
    }
    results.append(current, next);
    return results;
}

int main(int argc, const char *argv[]) {
	gpmdir.append("/.gpm");
	CURL *curl = curl_easy_init();
	string arg = argv[0];
	string initialArg = argv[1];
	if (initialArg == "install" || initialArg == "i") {
		for (int a = 2; a < argc; a++) {
			currentArg = argv[a];
			cout << "\033[00;32mPackage \"" << currentArg << "\" found, installing...\033[0m\n";
			pkgURL = "https://raw.githubusercontent.com/nsandman09/gpm-packages/master/" + currentArg + ".gpm";
			curl_easy_setopt(curl, CURLOPT_URL, pkgURL.c_str()); 
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write);
			curl_easy_perform(curl);
			if (returnedText == "Not Found") {
				cout << "\033[00;31mPackage \"" << currentArg << "\" not found, skipping...\033[0m\n";
			} else {
				string cfile = gpmdir + "/tmp/" + currentArg;
				rapidjson::Document file;
				file.Parse(returnedText.c_str());
				rapidjson::Value &url = file["url"];
				rapidjson::Value &cmds = file["commands"];
				returnedText = "";
				curl_easy_setopt(curl, CURLOPT_URL, url.GetString()); 
				curl_easy_perform(curl);
				ofstream dl;
				dl.open(cfile);
				dl << returnedText;
				dl.close();
				for (int b = 0; b < cmds.Size(); b++) {
					currentCommand = cmds[b].GetString();
					currentCommand = replaceAll(currentCommand, "{FILENAME}", cfile);
					currentCommand = replaceAll(currentCommand, "{GPMDIR}", gpmdir);
					system(currentCommand.c_str());
				}
				string mvChar = "mv " + cfile + " " + gpmdir + "/installed";
				string tmpDir = "exec rm -rf " + gpmdir + "/tmp/*";
				system(mvChar.c_str());
				system(tmpDir.c_str());
				fstream installed;
				installed.open(gpmdir + "/installed/_installed.gpi", fstream::app);
				installed << currentArg;
				installed << "\n";
				installed.close();
			}
			returnedText = "";
		}
		cout << "Done.\n";
	} else {
		cout << "\033[00;31mUnknown parameter \033[1;31m" + initialArg + " \033[0m\n";
	}
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	return 0;
}