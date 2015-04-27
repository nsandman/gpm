/* main.cpp - Noah Sandman
   04/24/2015 */

#include <cstring>
#include <fstream>
#include <iostream>
#include <curl/curl.h>

#include "main.h"
#include "pyembed/pyembed.h"
#include "rapidjson/document.h"
using namespace std;

int main(int argc, char *argv[]) {
	string GPMDIR = strcat(getenv("HOME"), "/.gpm");
	CURL *curl = curl_easy_init();
	/* Use callback to save cURL output to variable */
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, nFunctions::curlToVar);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2366.0 Safari/537.36");
	if (argc > 1) {
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
					if (strcmp(curlResult.c_str(), "Not Found") != 0) {
						cout << "\033[0;32mPackage \"" << ca << "\" found, installing...\033[0m\n";
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
						installed.open(string(GPMDIR + "/installed/_installed.gpi").c_str(), ios::app);
						installed << ca << endl;
						installed.close();
					} else {
						cout << "\033[0;31mPackage \"" << ca << "\" not found, skipping...\033[0m\n";
					}
				} else {
					cout << "\033[0;31mPackage \"" << ca << "\" already installed!\033[0m\n";
				}
			}
			/* Clean up! */
			curl_easy_cleanup(curl);
			curl_global_cleanup();
		} 
		else if (strcmp(argOne, "remove") == 0 || strcmp(argOne, "r") == 0 || strcmp(argOne, "delete") == 0 || strcmp(argOne, "d") == 0 || strcmp(argOne, "uninstall") == 0 || strcmp(argOne, "u") == 0) {
			for (int a = 2; a < argc; a++) {
				bool foundToRem = false;
				char *currentArg = argv[a];
				ifstream installed;
				installed.open(iDir);
				string line2;
				for (int b = 0; getline(installed, line2); b++) {
					if (strcmp(line2.c_str(), currentArg) == 0) {
						cout << "\033[0;32mPackage \"" << currentArg << "\" found, removing...\033[0m\n";
						remove((GPMDIR + "/installed/" + currentArg).c_str());
						try {
							pyembed::Python toDelete(0, argv); /* New Python interpreter */
							long returned = 0; /* Variable returned value is saved to */
							pyembed::Arg_map args; /* "Argmap" to give to function */
							/* Add three values to Argmap */
							args[string(GPMDIR + "/installed/_installed.gpi").c_str()] = pyembed::Py_string; /* to_open */
							args[currentArg] = pyembed::Py_string; /* look_for */
							toDelete.load("rmfgpi"); /* Load rmfgpi script ("module") */
							/* Call function delete_gpi with args from argmap and save to "returned" */
							toDelete.call("delete_gpi", args, returned);
						} catch (pyembed::Python_exception ex) {
							cerr << ex.what() << endl; /* Any errors to stderr */
						}
						foundToRem = true;
						break;
					}
				}
				installed.close();
				if (!foundToRem) {
					cout << "\033[0;31mPackage \"" << currentArg << "\" not found, skipping...\033[0m\n";
				}
			}
		}
		else if (strcmp(argOne, "list") == 0 || strcmp(argOne, "l") == 0) {
			ifstream installed;
			installed.open(iDir);
			string iOutput;
			string cLine;
			for (int i = 0; getline(installed, cLine); i++) {
				iOutput.append(cLine + "\n");
			}
			cout << "\033[1;30mINSTALLED PACKAGES\n==================\033[0m\n" << iOutput << endl;
			exit(0); /* Prevent "Done." from being printed */
		}
		else {
			cout << "\033[0;31mUnknown param " << argOne << "!\033[0m\n";
			exit(0);
		}
	} else {
		cout << "\033[0;31mNo params given!\033[0m\n";
		exit(0);
	}
	cout << "Done.\n";
	return 0;
}