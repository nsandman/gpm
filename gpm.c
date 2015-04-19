/*
        Project         
 ––––––––––––––––––––––
|   ____ ____  __  __  |
|  / ___|  _ \|  \/  | |
| | |  _| |_) | |\/| | |
| | |_| |  __/| |  | | |
|  \____|_|   |_|  |_| |
|                      |
|    by Noah Sandman   |
 ––––––––––––––––––––––
*/
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <curl/curl.h>

/* NOTE: None of this code is tested with anything other than GCC 4.9
   on OSX 10.10 Yosemite (installed with Homebrew) with the C11 setti
   -ng. This probably will not work with other versions because of st
   -ring formatting done weirdly and VLAs with malloc. */

char *pkgURL;

int main(int argc, char *argv[]) {
	CURL *curl;
	CURLcode res;
	
	char *gpmdir = strcat(getpwuid(getuid())->pw_dir, "/.gpm"); /* Get GPM file dir (~/.gpm) */
	if (strcmp(argv[1], "install") == 0 || strcmp(argv[1], "i") == 0) {
		for (int a = 2; a < argc; a++) {
			pkgURL = malloc(68 + strlen(argv[a]));
			curl = curl_easy_init();
			printf("\033[0;32mInstalling package %s...\n\033[0m", argv[a]);
			strcat(pkgURL, "https://raw.githubusercontent.com/nsandman09/gpm-packages/master/");
			strcat(pkgURL, argv[a]);
			strcat(pkgURL, ".gpm");
			if (curl) {
				curl_easy_setopt(curl, CURLOPT_URL, pkgURL);
				res = curl_easy_perform(curl);
				curl_easy_cleanup(curl);
				if (res == NULL) {
					printf(stderr, "\033[0;31mPackage \"%s\" not found!\033[0m", argv[a]);
				}
			}
			pkgURL = "";
		}
	}
	else {
		/* Otherwise, print "unknown command" error */
		printf(stderr, "\033[0;31mUnknown command: %s %s \033[0m\n", argv[0], argv[1]);
	}
	return 0;
}