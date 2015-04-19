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
#include <iostream>
using namespace std;

int main(int argc, const char *argv[]) {
	string arg = argv[0];
	string initialArg = argv[1];
	if (initialArg == "install" || initialArg == "i") {
		for (int a = 2; a < argc; a++) {
			string currentArg = argv[a];
			cout << "\033[00;32mInstalling package \"" << currentArg << "\"...\033[0m\n";
				string pkgURL = "https://github.com/nsandman09/gpm-packages/blob/master/" + currentArg + ".gpm";
				cout << pkgURL;
		}
	} else {
		cout << "\033[00;31mCommand \033[1;31m" + arg + " " + initialArg + " \033[0m\033[00;31mnot found.\033[0m\n";
	}
}