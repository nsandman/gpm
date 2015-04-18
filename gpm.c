#include <stdio.h>

int main(int argc, const char *argv[]) {
	const char *mainCmd = argv[1];
	if (strcmp(mainCmd, "install") == 0) {
		for (int a = 2; a < argc; a++) {
			printf("\033[0;32mInstalling package %s...\n\033[0m", argv[a]);

		}
	}
	else {
		printf("\033[0;31mUnknown command: %s %s \033[0m\n", argv[0], mainCmd);
	}
	return 0;
}