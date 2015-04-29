#ifndef _INCLUDED_MAIN_H
#define _INCLUDED_MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

#include "json.h"

char *CurlResult;		/* Blank string for cURL result */
char *currentChunk;
char *dlUrl;
cJSON *jCurlParse;
cJSON *cmds;
CURL *curl;
int timesRun; 				/* How many times have we run the function? */

size_t curlToVar(void *ptr, size_t size, size_t nmemb, ...) {
	currentChunk = malloc(strlen((char*)ptr)); 					/* Save value of ptr to variable */
	strcpy(currentChunk, (char*)ptr);
	if (timesRun == 0) {							/* If first time, use malloc() and strcpy() */
		CurlResult = malloc(strlen(currentChunk));
		strcpy(CurlResult, currentChunk);
	} else {																	/* Else, use realloc() and strncat() */
		CurlResult = realloc(CurlResult, strlen(CurlResult) + strlen(currentChunk));
		strncat(CurlResult, currentChunk, size*nmemb); 
	}
	timesRun++;
	return size * nmemb; 			/* Return bytes left to write (for cURL) */
}

void clearVar(void *varToClear, int reset) {
	if (reset == 1) timesRun = 0;				/* If resetting for cUrl, set timesRun to 0 */	
	memset(&varToClear, 0, sizeof(varToClear));
}

void cleanupMain() {				/* Free all malloc'd pointers */
	free(currentChunk);
	free(CurlResult);
}

#endif