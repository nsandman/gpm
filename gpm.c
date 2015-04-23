#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gpm.h"
#include "cjson/cjson.h"
#include "curl/curl.h"

struct fp {
};

int main(int argc, char *argv[]) {
	char *initArg = argv[1];
	CURL *curl = curl_easy_init();
	if (strcmp(initArg, "install") == 0 || strcmp(initArg, "i") == 0) {
		for (int args = 2; args < argc; args++) {
			char *cArg = argv[args];
			char *ghUrl = malloc(68 + sizeof(cArg));
			strcat(ghUrl, "https://raw.githubusercontent.com/nsandman09/gpm-packages/master/");
			strcat(ghUrl, cArg);
			strcat(ghUrl, ".gpm");
			curl_easy_setopt(curl, CURLOPT_URL, ghUrl);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlToVar);
			curl_easy_perform(curl);
			if (strcmp(curlResponse, "Not Found") != 0) {
				printf("\033[0;32mPackage \"%s\" found, installing...\033[0m\n", cArg);
			} else {
				printf("\033[0;31mPackage \"%s\" not found, skipping...\033[0m\n", cArg);
			}
		}
	} else {
		fprintf(stderr, "\033[0;31mUnknown paramter \"%s\"!\033[0m\n", initArg);
	}
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	return 0;
}