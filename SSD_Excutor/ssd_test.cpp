#include "gmock/gmock.h"
#include "ssd.h"
#include "SSDHelper.h"
#include "command_buffer_manager.h"
#include <filesystem>

#ifdef _DEBUG
namespace fs = std::filesystem;

class CommandParserMock : public CommandParser
{
public:
	MOCK_METHOD(CommandInfo, parse, (int argc, char* argv[]), (override));
};

class SSDTS : public testing::Test
{
protected:
	void SetUp() override
	{
		ssd = std::make_shared<SSD>(&commandParserMock);
		ssdHelper.resetSSD();
	}

public:
	testing::NiceMock<CommandParserMock> commandParserMock;
	std::shared_ptr<SSD> ssd;
	int dummyArgc{ 0 };
	char** dummyArgv{ nullptr };
	SSDHelper ssdHelper;
};

TEST_F(SSDTS, WriteAndEraseSimpleTC1)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 0, 4}))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 0, 2 }));

	for (int i = 0; i < 2; i++)
	{
		ssd->run(dummyArgc, dummyArgv);
	}

	std::string actual;

	actual = ssdHelper.directAccessNand(0);
	EXPECT_EQ("", actual);

	actual = ssdHelper.directAccessNand(1);
	EXPECT_EQ("", actual);
}

TEST_F(SSDTS, WriteAndEraseSimpleTC2)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 0, 0x7 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 1, 0x77 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 2, 0x777 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 1, 2 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 3, 0x7777 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 10, 0xFFFF }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 5, 0x777777 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 4, 0x77777 }));
	
	for (int i = 0; i < 8; i++)
	{
		ssd->run(dummyArgc, dummyArgv);
	}

	std::string actual;

	actual = ssdHelper.directAccessNand(0);
	EXPECT_EQ(ssdHelper.makeExpectedNandString(0, 0x7), actual);

	actual = ssdHelper.directAccessNand(1);
	EXPECT_EQ(ssdHelper.makeExpectedNandString(1, 0x0), actual);

	actual = ssdHelper.directAccessNand(2);
	EXPECT_EQ(ssdHelper.makeExpectedNandString(2, 0x0), actual);

	actual = ssdHelper.directAccessNand(3);
	EXPECT_EQ(ssdHelper.makeExpectedNandString(3, 0x7777), actual);

	actual = ssdHelper.directAccessNand(10);
	EXPECT_EQ(ssdHelper.makeExpectedNandString(10, 0xFFFF), actual);

}

TEST_F(SSDTS, DISABLED_SsdReadAfterWrite)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 1, (unsigned int)0x1 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 2, (unsigned int)0x2 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_READ), 1, 0xFFFFFFFF }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_READ), 2, 0xFFFFFFFF }));

	ssd->run(dummyArgc, dummyArgv);
	ssd->run(dummyArgc, dummyArgv);
	ssd->run(dummyArgc, dummyArgv);
	EXPECT_EQ(ssdHelper.getReadResultFromFile(), "0x00000001");
	ssd->run(dummyArgc, dummyArgv);
	EXPECT_EQ(ssdHelper.getReadResultFromFile(), "0x00000002");
}

TEST_F(SSDTS, SSDInavlidCommandShouldPrintOutputError)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 0xFFFFFFFF, 0xFFFFFFFF }));
	ssd->run(dummyArgc, dummyArgv);
	EXPECT_EQ(ssdHelper.getReadResultFromFile(), "ERROR");
}
#endif