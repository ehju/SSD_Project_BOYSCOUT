#include "gmock/gmock.h"
#include "CommandParser.h"
using namespace testing;

class CPFixture : public Test {
public:
	int checkCommandType(const string cmd)
	{
		return cp.getCommandType(cmd);
	}
private:
	CommandParser cp;
};

TEST_F(CPFixture, BasicInvalidCommand) {
	EXPECT_EQ(CMD_NOT_SUPPORTED, checkCommandType("invalid"));
}
TEST_F(CPFixture, BasicValidCommand) {
	EXPECT_EQ(CMD_BASIC_WRITE, checkCommandType("write"));
	EXPECT_EQ(CMD_BASIC_READ, checkCommandType("read"));
	EXPECT_EQ(CMD_BASIC_EXIT, checkCommandType("exit"));
	EXPECT_EQ(CMD_BASIC_HELP, checkCommandType("help"));
	EXPECT_EQ(CMD_BASIC_FULLWRITE, checkCommandType("fullwrite"));
	EXPECT_EQ(CMD_BASIC_FULLREAD, checkCommandType("fullread"));
}

TEST(CPTest, RunCommandCallExit) {
	CommandParser cp;
	const string cmdline = "exit";
	EXPECT_EQ(CMD_BASIC_EXIT, cp.runCommand(cmdline));
}

TEST(CPTest, RunCommandCallWrite) {
	CommandParser cp;
	const string cmdline = "write 3 0xAAAABBBB";
	EXPECT_EQ(CMD_BASIC_WRITE, cp.runCommand(cmdline));
}
TEST(CPTest, RunCommandCallRed) {
	CommandParser cp;
	const string cmdline = "read 0";
	EXPECT_EQ(CMD_BASIC_READ, cp.runCommand(cmdline));
}
TEST(CPTest, RunCommandWritePassDefault) {
	CommandParser cp;
	const string cmdline = "write 3 0xAAAABBBB";

	std::stringstream buffer;
	std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
	int expected = CMD_BASIC_WRITE;
	int actual = cp.runCommand(cmdline);

	std::cout.rdbuf(old);
	string output = buffer.str();
	EXPECT_EQ(expected, actual);
	EXPECT_EQ(output, "[Write] Done\n");
}
TEST(CPTest, RunCommandReadPassDefault) {
	CommandParser cp;
	const string cmdline = "read 3";

	std::stringstream buffer;
	std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
	int expected = CMD_BASIC_READ;
	int actual = cp.runCommand(cmdline);

	std::cout.rdbuf(old);
	string output = buffer.str();
	EXPECT_EQ(expected, actual);
	EXPECT_EQ(output, "[Read] LBA 3 : 0x00000000\n");
}
TEST(CPTest, RunCommandFullReadPassDefault) {
	CommandParser cp;
	const string cmdline = "fullread";

	std::stringstream buffer;
	std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
	int expected = CMD_BASIC_FULLREAD;
	int actual = cp.runCommand(cmdline);

	std::cout.rdbuf(old);
	string output = buffer.str();
	EXPECT_EQ(expected, actual);
}