#include "gmock/gmock.h"
#include "ShellRunner.h"
using namespace testing;

class SRFixture : public Test {
public:
	void checkCommandType(const string cmd, int expected)
	{
		EXPECT_EQ(expected, shell.runCommand(cmd));
	}
private:
	ShellRunner shell;
}; 

TEST_F(SRFixture, RunCommandReturnType) {
	checkCommandType(" ", CMD_NOT_SUPPORTED);
	checkCommandType("exit", CMD_BASIC_EXIT);
	checkCommandType("write 3 0xAAAABBBB", CMD_BASIC_WRITE);
	checkCommandType("read 0", CMD_BASIC_READ);
	checkCommandType("erase 0 1", CMD_BASIC_ERASE);
	checkCommandType("erase_range 0 1", CMD_BASIC_ERASE_RANGE);
	checkCommandType("fullread", CMD_BASIC_FULLREAD);
	checkCommandType("fullwrite 0xAAAABBBB", CMD_BASIC_FULLWRITE);
	checkCommandType("1_FullWriteAndReadCompare", CMD_TS_FullWriteAndReadCompare);
	checkCommandType("2_PartialLBAWrite", CMD_TS_PartialLBAWrite);
	checkCommandType("3_WriteReadAging", CMD_TS_WriteReadAging);
	checkCommandType("4_EraseAndWriteAging", CMD_TS_EraseWriteAging);
}

TEST_F(SRFixture, DISABLED_RunCommandWritePassDefault) {
	ShellRunner shell;
	const string cmdline = "write 3 0xAAAABBBB";

	std::stringstream buffer;
	std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
	int expected = CMD_BASIC_WRITE;
	int actual = shell.runCommand(cmdline);

	std::cout.rdbuf(old);
	string output = buffer.str();
	EXPECT_EQ(expected, actual);
	EXPECT_EQ(output, "[Write] Done\n");
}
TEST_F(SRFixture, DISABLED_RunCommandReadPassDefault) {
	ShellRunner shell;;
	const string cmdline = "read 3";

	std::stringstream buffer;
	std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
	int expected = CMD_BASIC_READ;
	int actual = shell.runCommand(cmdline);

	std::cout.rdbuf(old);
	string output = buffer.str();
	EXPECT_EQ(expected, actual);
	EXPECT_EQ(output, "[Read] LBA 3 : 0x00000000\n");
}

TEST_F(SRFixture, RunHelpCommand) {
	ShellRunner shell;;
	const string cmdline = "help";

	vector<string> containsString = { "BOYSCOUT",
		"¹Ú¼¼¿î",
		"ÀÌ½ÂÇö/ÁÖÀºÇý/Á¤Áø¼·/ÇãÈÆ/Á¤ÇýÁø",
		"write","read","fullwrite","fullread" ,"exit","help",
		"1_FullWriteAndReadCompare",
		"2_PartialLBAWrite",
		"3_WriteReadAging", };

	std::stringstream buffer;
	std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
	int expected = CMD_BASIC_HELP;
	int actual = shell.runCommand(cmdline);

	std::cout.rdbuf(old);
	string output = buffer.str();
	std::cout << output;
	EXPECT_EQ(expected, actual);
	for (string str : containsString)
	{
		size_t index = output.find(str);
		EXPECT_THAT(index, Ne(-1));
	}
}