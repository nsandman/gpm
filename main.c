#include "main.h"

int main(int argc, char *argv[]) {
	gpmdir = strcat(getenv("HOME"), "/.gpm");
	dlUrl = malloc(1);
	downloadedFileName = malloc(1);
	toMove = malloc(1);
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlToVar);
	if (argc > 1) {
		char *argOne = argv[1];
		if (strcmp(argOne, "install") == 0 || strcmp(argOne, "i") == 0) {
			if (argc > 2) {
				for (int a = 2; a < argc; a++) {
					char *currentArg = argv[a];
					dlUrl = realloc(dlUrl, 68 + strlen(currentArg));
					strcpy(dlUrl, "https://raw.githubusercontent.com/nsandman09/gpm-packages/master/");
					strcat(dlUrl, currentArg);
					strcat(dlUrl, ".gpm");
					clearVar(CurlResult, 1);
					curl_easy_setopt(curl, CURLOPT_URL, dlUrl);
					curl_easy_perform(curl);
					if (strcmp(CurlResult, "Not Found") != 0) {
						printf("\033[0;32mPackage \"%s\" found, installing...\033[0m\n", currentArg);
						jCurlParse = cJSON_Parse(CurlResult);
						#ifndef _WIN32
						cmds = cJSON_GetObjectItem(jCurlParse,"commands");
						#else
						cmds = cJSON_GetObjectItem(jCurlParse,"win32");
						#endif
						urlFromJson = cJSON_Print(cJSON_GetObjectItem(jCurlParse,"url"));
						clearVar(CurlResult, 1);
						curl_easy_setopt(curl, CURLOPT_URL, urlFromJson);
						curl_easy_perform(curl);
						downloadedFileName = realloc(downloadedFileName, strlen(gpmdir) + strlen(currentArg) + 5);
						strcpy(downloadedFileName, gpmdir);
						strcat(downloadedFileName, "/tmp/");
						downloadedFile = fopen(strcat(downloadedFileName, currentArg), "w");
						fprintf(downloadedFile, "%s", CurlResult);
						fclose(downloadedFile);
						for (int b = 0; b < cJSON_GetArraySize(cmds); b++) {
							char *tmpCmd = cJSON_Print(cJSON_GetArrayItem(defaults, b));
							replaceAll(tmpCmd, "{FILENAME}", downloadedFileName);
							replaceAll(tmpCmd, "{GPMDIR}", gpmdir);
							replaceAll(tmpCmd, "\\\"", "\"");			/* Replace escaped quotes with legit ones */
							system(tmpCmd);
						}
						toMove = realloc(toMove, strlen(downloadedFileName) + 6);
						strcpy(toMove, downloadedFileName);
						replaceAll(toMove, "/tmp/", "/installed/");
						rename(downloadedFileName, toMove);
					} else {
						packageFound = 0;
						printf("\033[0;31mPackage \"%s\" not found, skipping...\033[0m\n", currentArg);
					}
				}
			} else printHelp("\033[1;31mYou must enter a package to install!\033[0m");
		} else if (strcmp(argOne, "help") == 0 || strcmp(argOne, "h") == 0) printHelp("");
		else {
			printf("\033[1;31mUnknown param \"%s\"!\033[0m\n", argOne);
			printHelp("");
		}
	} else printHelp("");
	/* Run function cleanups */
	cleanupMain();
	cJSON_Delete(jCurlParse);
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	printf("Done.\n");
	return 0;
}
