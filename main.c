#include "main.h"

int main(int argc, char *argv[]) {
	const char *gpmdir = strcat(getenv("HOME"), "/.gpm");
	dlUrl = malloc(1);
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlToVar);
	if (argc > 1) {
		if (strcmp((char*)argv[1], "install") == 0 || strcmp((char*)argv[1], "i") == 0) {
			for (int a = 2; a < argc; a++) {
				char *currentArg = argv[a];
				dlUrl = realloc(dlUrl, 68 + strlen(currentArg));
				strcat(dlUrl, "https://raw.githubusercontent.com/nsandman09/gpm-packages/master/");
				strcat(dlUrl, currentArg);
				strcat(dlUrl, ".gpm");
				clearVar(curlResult);
				curl_easy_setopt(curl, CURLOPT_URL, dlUrl);
				curl_easy_perform(curl);
				jCurlParse = cJSON_Parse(curlResult);
				cmds = cJSON_GetObjectItem(jCurlParse,"commands");
				char *urlFromJson = cJSON_Print(cJSON_GetObjectItem(jCurlParse,"url"));
				clearVar(curlResult);
				curl_easy_setopt(curl, CURLOPT_URL, urlFromJson);
				curl_easy_perform(curl);
				for (int b = 0; b < cJSON_GetArraySize(cmds); b++) {
					printf("%s\n", cJSON_Print(cJSON_GetArrayItem(cmds, b)));
					/* system(cJSON_Print(cJSON_GetArrayItem(cmds, b))); */
				}
			}
		} else {
			printf("Unknown param \"%s\"!\n", (char*)argv[1]);
		}
	} else {
		printf("No params entered!\n");
	}
	/* Run function cleanups */
	cleanupMain();
	cJSON_Delete(jCurlParse);
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	return 0;
}