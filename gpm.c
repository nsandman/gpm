#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <curl/curl.h>

int main(int argc, const char *argv[]) {
	// Get GPM file dir (~/.gpm)
	char *gpmdir = strcat(getpwuid(getuid())->pw_dir, "/.gpm");
	if (strcmp(argv[1], "install") == 0) { // See if first argument is "install"
		// For ever arguments that is a package name
		for (int a = 2; a < argc; a++) {
			char *pkg = argv[a]; // argv[a] for later usage
			printf("\033[0;32mInstalling package %s...\n\033[0m", pkg);
			// Declare filename here so we don't give fopen the arguments
			char *file = "%s/pkg/%s.gpm", gpmdir, argv[a];
			char *pkgfile = fopen(file, "r"); // Open package file in ~/.gpm/pkg/{PACKAGENAME}.gpm
		}
	}
	else {
		// Otherwise, print "unknown command" error
		printf(stderr, "\033[0;31mUnknown command: %s %s \033[0m\n", argv[0], argv[1]);
	}
	// Exit with error code 0
	return 0;
}