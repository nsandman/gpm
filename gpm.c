#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gpm.h"
//#include "cjson.h"
#include "curl/curl.h"

struct fp {
};

int main(int argc, char *argv[]) {
	char *initArg = argv[1];
	if (strcmp(initArg, "install") == 0 || strcmp(initArg, "i") == 0) {
		char curlData = "";
		CURL *curl = curl_easy_init();
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
				printf("Package \"%s\" found, installing...\n", cArg);
			} else {
				printf("Package \"%s\" not found, skipping...\n", cArg);
			}
		}
	} else {
		fprintf(stderr, "Unknown paramter \"%s\"!\n", initArg);
	}
	return 0;
}