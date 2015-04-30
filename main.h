#ifndef _INCLUDED_MAIN_H
#define _INCLUDED_MAIN_H

// System headers:
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

// Local headers:
#include "json.h"

// Macros:
#define MAX_L 4096

// Ints:
char *dlUrl;
char *gpmdir;
char *currentChunk;
char *CurlResult;
char *downloadedFileName;
char *urlFromJson;
FILE *downloadedFile;
cJSON *jCurlParse;
cJSON *cmds;
CURL *curl;
int ran = 1;
int timesRun;

size_t curlToVar(void *ptr, size_t size, size_t nmemb, ...) {
/*
 * This is a callback function for libcURL that
 * saves the output to a variable (CurlResult)
 * instead of printing it (like it does by default).
 */
	currentChunk = malloc(strlen((char*)ptr)); 					/* Save value of ptr to variable */
	strcpy(currentChunk, (char*)ptr);
	if (timesRun == 0) {							/* If first time, use malloc() and strcpy() */
		CurlResult = malloc(strlen(currentChunk));
		strcpy(CurlResult, currentChunk);
	} else {																	/* Else, use realloc() and strncat() */
		CurlResult = realloc(CurlResult, strlen(CurlResult) + strlen(currentChunk));
		strncat(CurlResult, currentChunk, size * nmemb);		/* Only concatenate proper number to avoid overrun */
	}
	timesRun++;
	return size * nmemb; 			/* Return bytes left to write (for cURL) */
}

void replaceAll(char * o_string, char * s_string, char * r_string) {
      char buffer[MAX_L];
      char * ch;

      if(!(ch = strstr(o_string, s_string))) return;
      strncpy(buffer, o_string, ch-o_string);
      buffer[ch-o_string] = 0;
      sprintf(buffer+(ch - o_string), "%s%s", r_string, ch + strlen(s_string));
      o_string[0] = 0;
      strcpy(o_string, buffer);
      return replaceAll(o_string, s_string, r_string);
 }

void clearVar(void *varToClear, int reset) {
	/* Clear a var with memset, usually for cURL. */
	if (reset == 1) timesRun = 0;				/* If resetting for cUrl, set timesRun to 0 */	
	memset(&varToClear, 0, sizeof(varToClear));
}

void cleanupMain() {				/* Free all malloc'd pointers */
	if (ran == 1) {
		free(dlUrl);			/* This must be done manually for some reason */
		free(downloadedFileName);
	}
	free(currentChunk);
	free(CurlResult);
}

#endif