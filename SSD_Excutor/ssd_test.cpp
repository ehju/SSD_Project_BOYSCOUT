#include "gmock/gmock.h"
#include "ssd.cpp"

class CommandParserMock : public CommandParser
{
public:
	MOCK_METHOD(CommandInfo, parse, (int argc, char* argv[]), (override));
};

TEST(SSDTS, TC1)
{
	CommandParserMock commandParserMock;
	Write writeCommand;
	Read readCommand;

	SSD ssd{ &commandParserMock, &writeCommand, &readCommand };
	
	int dummyArgc = 0;
	char* argv1[] = { nullptr };

	CommandInfo mockReturned = { 0, 0, 1 };
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillOnce(testing::Return(mockReturned));

	ssd.run(dummyArgc, argv1);


	std::string filename = "ssd_nand.txt";
	std::ifstream file(filename);
	std::string line;

	std::string expected = "0 0x00000001";

	if (file.is_open())
	{
		getline(file, line);
		EXPECT_EQ(expected, line);
		file.close();
	}
	else
	{
		FAIL();
	}

}