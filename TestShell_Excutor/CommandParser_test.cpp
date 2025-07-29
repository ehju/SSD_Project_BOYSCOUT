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