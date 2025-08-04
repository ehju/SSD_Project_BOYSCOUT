#include "gmock/gmock.h"
#include "command_buffer_manager.h"
#include "ssd.h"
#include "SSDHelper.h"

#ifdef _DEBUG
class CommandParserMock : public CommandParser
{
public:
	MOCK_METHOD(CommandInfo, parse, (int argc, char* argv[]), (override));
};


class CommandBufferMangerTS : public testing::Test
{
protected:
	void SetUp() override
	{
		ssd = std::make_shared<SSD>(&commandParserMock);
		CommandBufferManager::getInstance().initialize();
		ssdHelper.resetSSD();
	}

	void TearDown() override
	{
	}

public:

	testing::NiceMock<CommandParserMock> commandParserMock;
	std::shared_ptr<SSD> ssd;
	int dummyArgc{ 0 };
	char** dummyArgv{ nullptr };
	SSDHelper ssdHelper;
};

TEST_F(CommandBufferMangerTS, TC1)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillRepeatedly(testing::Return(CommandInfo{ 0, 1, 4 }));

	ssd->run(dummyArgc, dummyArgv);

	EXPECT_EQ(ssdHelper.makeCommandBufferName(0, CommandInfo{0, 1, 4}), ssdHelper.getBufferName(0));
}

TEST_F(CommandBufferMangerTS, TC2)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 1, 0xFFFFFFFF}))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 2, 0x12345678 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 3, 0x00000001 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 4, 0xABCE1234 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 5, 0xAAAAAAAA }));
	
	for (int i = 0; i < 5; i++)
	{
		ssd->run(dummyArgc, dummyArgv);
	}

	EXPECT_EQ(ssdHelper.makeCommandBufferName(0, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 1, 0xFFFFFFFF }), ssdHelper.getBufferName(0));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(1, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 2, 0x12345678 }), ssdHelper.getBufferName(1));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(2, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 3, 0x00000001 }), ssdHelper.getBufferName(2));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(3, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 4, 0xABCE1234 }), ssdHelper.getBufferName(3));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(4, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 5, 0xAAAAAAAA }), ssdHelper.getBufferName(4));
}

TEST_F(CommandBufferMangerTS, OptimizeCommandBUfferTC1)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 0, 0x11112222 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 0, 2 }));

	for (int i = 0; i < 2; i++)
	{
		ssd->run(dummyArgc, dummyArgv);
	}

	EXPECT_EQ(ssdHelper.makeCommandBufferName(0, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 0, 2 }), ssdHelper.getBufferName(0));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(1, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 2, 0x12345678 }), ssdHelper.getBufferName(1));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(2, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 3, 0x00000001 }), ssdHelper.getBufferName(2));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(3, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 4, 0xABCE1234 }), ssdHelper.getBufferName(3));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(4, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 5, 0xAAAAAAAA }), ssdHelper.getBufferName(4));
}

TEST_F(CommandBufferMangerTS, OptimizeCommandBUfferTC2)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 0, 2 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 0, 0x00001111 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 1, 0x11112222 }));

	for (int i = 0; i < 3; i++)
	{
		ssd->run(dummyArgc, dummyArgv);
	}

	EXPECT_EQ(ssdHelper.makeCommandBufferName(0, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 1, 0x11112222 }), ssdHelper.getBufferName(0));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(1, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 0, 0x00001111 }), ssdHelper.getBufferName(1));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(2, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 3, 0x00000001 }), ssdHelper.getBufferName(2));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(3, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 4, 0xABCE1234 }), ssdHelper.getBufferName(3));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(4, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 5, 0xAAAAAAAA }), ssdHelper.getBufferName(4));
}

TEST_F(CommandBufferMangerTS, OptimizeCommandBUfferTC3)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 0, 0x00001111 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 0, 0x88889999 }));

	for (int i = 0; i < 2; i++)
	{
		ssd->run(dummyArgc, dummyArgv);
	}

	EXPECT_EQ(ssdHelper.makeCommandBufferName(0, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 0, 0x88889999 }), ssdHelper.getBufferName(0));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(1, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 1, 0x11112222 }), ssdHelper.getBufferName(1));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(2, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 3, 0x00000001 }), ssdHelper.getBufferName(2));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(3, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 4, 0xABCE1234 }), ssdHelper.getBufferName(3));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(4, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 5, 0xAAAAAAAA }), ssdHelper.getBufferName(4));
}

TEST_F(CommandBufferMangerTS, OptimizeCommandBUfferTC4)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 0, 2 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 0, 0x00001111 }));

	for (int i = 0; i < 2; i++)
	{
		ssd->run(dummyArgc, dummyArgv);
	}

	EXPECT_EQ(ssdHelper.makeCommandBufferName(0, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 0, 2 }), ssdHelper.getBufferName(0));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(1, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 0, 0x00001111 }), ssdHelper.getBufferName(1));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(2, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 3, 0x00000001 }), ssdHelper.getBufferName(2));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(3, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 4, 0xABCE1234 }), ssdHelper.getBufferName(3));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(4, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 5, 0xAAAAAAAA }), ssdHelper.getBufferName(4));
}

TEST_F(CommandBufferMangerTS, OptimizeCommandBUfferTC5)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 0, 8 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 8, 3 }))	
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 11, 5 }));

	for (int i = 0; i < 3; i++)
	{
		ssd->run(dummyArgc, dummyArgv);
	}

	EXPECT_EQ(ssdHelper.makeCommandBufferName(0, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 10, 6 }), ssdHelper.getBufferName(0));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(1, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 0, 10 }), ssdHelper.getBufferName(1));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(2, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 3, 0x00000001 }), ssdHelper.getBufferName(2));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(3, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 4, 0xABCE1234 }), ssdHelper.getBufferName(3));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(4, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 5, 0xAAAAAAAA }), ssdHelper.getBufferName(4));
}

TEST_F(CommandBufferMangerTS, OptimizeCommandBUfferTC6)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 10, 4 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 12, 3 }));

	for (int i = 0; i < 2; i++)
	{
		ssd->run(dummyArgc, dummyArgv);
	}

	EXPECT_EQ(ssdHelper.makeCommandBufferName(0, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 10, 5 }), ssdHelper.getBufferName(0));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(1, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 0, 10 }), ssdHelper.getBufferName(1));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(2, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 3, 0x00000001 }), ssdHelper.getBufferName(2));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(3, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 4, 0xABCE1234 }), ssdHelper.getBufferName(3));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(4, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 5, 0xAAAAAAAA }), ssdHelper.getBufferName(4));
}

TEST_F(CommandBufferMangerTS, OptimizeCommandBUfferTC7)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 10, 1 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 11, 0x11111111 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 12, 1 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 13, 0x22222222 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 14, 1 }));

	for (int i = 0; i < 5; i++)
	{
		ssd->run(dummyArgc, dummyArgv);
	}

	EXPECT_EQ(ssdHelper.makeCommandBufferName(0, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 10, 5 }), ssdHelper.getBufferName(0));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(1, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 13, 0x22222222 }), ssdHelper.getBufferName(1));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(2, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 11, 0x11111111 }), ssdHelper.getBufferName(2));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(3, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 4, 0xABCE1234 }), ssdHelper.getBufferName(3));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(4, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 5, 0xAAAAAAAA }), ssdHelper.getBufferName(4));
}

TEST_F(CommandBufferMangerTS, OptimizeCommandBUfferTC8)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 10, 1 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 11, 0x11111111 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 12, 1 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 13, 0x22222222 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 14, 1 }));

	for (int i = 0; i < 5; i++)
	{
		ssd->run(dummyArgc, dummyArgv);
	}

	EXPECT_EQ(ssdHelper.makeCommandBufferName(0, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 10, 5 }), ssdHelper.getBufferName(0));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(1, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 13, 0x22222222 }), ssdHelper.getBufferName(1));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(2, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 11, 0x11111111 }), ssdHelper.getBufferName(2));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(3, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 4, 0xABCE1234 }), ssdHelper.getBufferName(3));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(4, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 5, 0xAAAAAAAA }), ssdHelper.getBufferName(4));
}


TEST_F(CommandBufferMangerTS, OptimizeCommandBUfferTC9)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 98, 2 }));

	for (int i = 0; i < 1; i++)
	{
		ssd->run(dummyArgc, dummyArgv);
	}

	EXPECT_EQ(ssdHelper.makeCommandBufferName(0, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 98, 2 }), ssdHelper.getBufferName(0));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(1, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 13, 0x22222222 }), ssdHelper.getBufferName(1));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(2, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 11, 0x11111111 }), ssdHelper.getBufferName(2));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(3, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 4, 0xABCE1234 }), ssdHelper.getBufferName(3));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(4, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 5, 0xAAAAAAAA }), ssdHelper.getBufferName(4));
}

TEST_F(CommandBufferMangerTS, OptimizeCommandBUfferTC10)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 10, 10 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 20, 10 }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 30, 0xAAAAAAAA }))
		.WillOnce(testing::Return(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 31, 9 }));


	for (int i = 0; i < 4; i++)
	{
		ssd->run(dummyArgc, dummyArgv);
	}

	EXPECT_EQ(ssdHelper.makeCommandBufferName(0, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 10, 10 }), ssdHelper.getBufferName(0));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(1, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 20, 10 }), ssdHelper.getBufferName(1));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(2, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 30, 0xAAAAAAAA }), ssdHelper.getBufferName(2));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(3, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 31, 9 }), ssdHelper.getBufferName(3));
	EXPECT_EQ(ssdHelper.makeCommandBufferName(4, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 5, 0xAAAAAAAA }), ssdHelper.getBufferName(4));
}
#endif