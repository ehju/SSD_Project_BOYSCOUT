#include "gmock/gmock.h"
#include "ShellRunner.h"
using std::string;
using std::cout;

#ifdef _DEBUG
int main() {
    ::testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}
#else
int main(int argc, char* argv[]) {
	string command;
	ShellRunner shell;

	while (true) {
		cout << "Shell> ";
		getline(std::cin, command);
		int type = shell.runCommand(command);

		switch (type) {
		case CMD_NOT_SUPPORTED:
			cout << "INVALID COMMAND\n";
			break;
		case CMD_BASIC_EXIT:
			return 0;
		}
	}

    return 0;
}
#endif