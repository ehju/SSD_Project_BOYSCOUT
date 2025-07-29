#include "gmock/gmock.h"
#include "CommandParser.h"
using namespace testing;

TEST(CParserTS, BasicInvalidCommand) {
    CommandParser cp;

    int actual = cp.getCommandType("invalid");
    EXPECT_EQ(-1, actual);
}