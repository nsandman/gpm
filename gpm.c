#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cjson.h"
#include "curl/curl.h"

/* By "falconindy" from http://ubuntuforums.org/archive/index.php/t-1435926.html */
#define BUFFER_SIZE (256 * 1024) /* 256kB */

struct write_result {
	char *data;
	int pos;
};

static size_t curl_write(void *ptr, size_t size, size_t nmemb, void *stream) {
	struct write_result *result = (struct write_result *)stream;
	if(result->pos + size * nmemb >= BUFFER_SIZE - 1) {
		fprintf(stderr, "curl error: too small buffer\n");
		return 0;
	}
	memcpy(result->data + result->pos, ptr, size * nmemb);
	result->pos += size * nmemb;
	return size * nmemb;
} 
/* End code by "falconindy" */

void print(char *toPrint) {
	printf(toPrint);
	printf("\n");
}

int main(int argc, char *argv[]) {
	CURL *curl = curl_easy_init();
	char *data = malloc(BUFFER_SIZE);
	char *initialArg = argv[1];
	/* "falconindy" */
	struct write_result write_result = {
		.data = data,
		.pos = 0
	};
	/* end */
	if (strcmp(initialArg, "install") == 0 || strcmp(initialArg, "i") == 0) {
		cJSON *parsedFile;
		for (int a = 2; a < argc; a++) {
			char *currentArg = argv[a];
			printf("\033[0;32mPackage \"");
			printf(currentArg);
			print("\" found, installing...\033[0m");
			char *pkgUrl = malloc(68 + sizeof(currentArg));
			strcat(pkgUrl, "https://raw.githubusercontent.com/nsandman09/gpm-packages/master/");
			strcat(pkgUrl, currentArg);
			strcat(pkgUrl, ".gpm");
			curl_easy_setopt(curl, CURLOPT_URL, pkgUrl); 
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &write_result);
			curl_easy_perform(curl);
			parsedFile = cJSON_Parse(data);
			char *rawUrl = cJSON_Print(cJSON_GetObjectItem(parsedFile, "url"));
			rawUrl++; /* Remove 1st character (a ") */
			rawUrl[strlen(rawUrl) - 1] = NULL; /* Remove last character (also a ") */
			print(rawUrl);
		}
	} else {
		printf("\033[0;31mUnknown paramter \"%s\"!\033[0m\n", initialArg);
	}
	/* Clean up */
	//free(pkgUrl);
	free(data);
	//cJSON_Delete(parsedFile);
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	return 0;
}