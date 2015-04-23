#include <stdlib.h>
#include "curl/curl.h"

char *curlResponse;

/* Inline functions :) */
void print(char *toPrint) {
	printf(toPrint);
	printf("\n");
}

size_t curlToVar(void *ptr, size_t size, size_t nmemb, ...) {
	curlResponse = strndup(ptr, (size_t)(size *nmemb));
	return size * nmemb;
}