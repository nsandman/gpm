#include "main.h"

int main(int argc, char *argv[]) {
	char *GPMDIR = strcat(getenv("HOME"), "/.gpm");
	CURL *curl = curl_easy_init();
	if (argc > 1) {
		if (strcmp((char*)argv[1], "install") == 0 || strcmp((char*)argv[1], "i") == 0) {
			for (int a = 2; a < argc; a++) {
				char *currentArg = argv[a];
				char *dlUrl = malloc(68 + strlen(currentArg));
				strcat(dlUrl, "https://raw.githubusercontent.com/nsandman09/gpm-packages/master/");
				strcat(dlUrl, currentArg);
				strcat(dlUrl, ".gpm");
				memset(&curlResult, 0, sizeof(curlResult));
				curl_easy_setopt(curl, CURLOPT_URL, dlUrl);
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlToVar);
				curl_easy_perform(curl);
			}
		} else {
			printf("Unknown param \"%s\"!\n", (char*)argv[1]);
		}
	} else {
		printf("No params entered!\n");
	}
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	return 0;
}