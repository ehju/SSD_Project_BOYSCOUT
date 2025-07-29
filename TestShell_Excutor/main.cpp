#include "gmock/gmock.h"
#include "CommandParser.h";
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
	CommandParser cp;
	int type;
	while (true) {
		cout << "Shell> ";
		getline(std::cin, command);
		type = cp.runCommand(command);

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