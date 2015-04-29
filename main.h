#ifndef _INCLUDED_MAIN_H
#define _INCLUDED_MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

#include "json-parser/json.h"

/* Blank string for cURL result */
char *curlResult;
int timesRun = 0; /* How many times have we run the function? */
size_t curlToVar(void *ptr, size_t size, size_t nmemb, ...) {
	char *currentChunk = (char*)ptr; /* Save value of ptr to variable */
	if (timesRun == 0) {
		curlResult = currentChunk; /* Avoid corruption if short */
	} else {
		if (timesRun == 1) {
			/* If second run, use malloc() */
			curlResult = malloc(strlen(currentChunk) + strlen(curlResult));
		} else {
			/* Else, use realloc() */
			curlResult = realloc(curlResult, (strlen(curlResult) + strlen(currentChunk)));
		}
		/* Append currentChunk to curlResult (strncat() to avoid weird numbers showing up) */
		strncat(curlResult, currentChunk, size*nmemb); 
	}
	timesRun++; /* Increment timesRun */
	return size * nmemb; /* Return bytes left to write (for cURL) */
}

#endif