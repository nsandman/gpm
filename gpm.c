#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <curl/curl.h>

int main(int argc, const char *argv[]) {
	char *gpmdir = strcat(getpwuid(getuid())->pw_dir, "/.gpm");
	if (strcmp(argv[1], "install") == 0) {
		for (int a = 2; a < argc; a++) {
			char *pkg = argv[a];
			printf("\033[0;32mInstalling package %s...\n\033[0m", pkg);
			char *file = "%s/pkg/%s.gpm", gpmdir, argv[a];
			char *pkgfile = fopen(file, "r");
		}
	}
	else {
		printf("\033[0;31mUnknown command: %s %s \033[0m\n", argv[0], argv[1]);
	}
	return 0;
}