#include "gmock/gmock.h"
#include "command_buffer_manager.h"
#include "ssd.h"

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
		CommandBufferManager::getInstance().clearCommandBuffer();
	}

	void TearDown() override
	{
		file.close();
	}

public:
	std::string makeCommandBufferName(unsigned int index, CommandInfo cmdInfo)
	{
		std::string line = "";
		std::string value = "";

		if (cmdInfo.command == static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE))
		{
			std::ostringstream ss;
			ss << "0x" << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << cmdInfo.value;
			value = ss.str();
			line = std::to_string(index) + "_" + "W" + "_" + std::to_string(cmdInfo.lba) + "_" + value;
		}
		else if (cmdInfo.command == static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE))
		{
			value = std::to_string(cmdInfo.value);
			line = std::to_string(index) + "_" + "E" + "_" + std::to_string(cmdInfo.lba) + "_" + value;
		}
		else
		{
			line = std::to_string(index) + "_empty";
		}
		return line;

	}

	testing::NiceMock<CommandParserMock> commandParserMock;
	std::shared_ptr<SSD> ssd;
	int dummyArgc{ 0 };
	char** dummyArgv{ nullptr };
	std::string nand{ "ssd_nand.txt" };
	fs::path folderPath = fs::current_path() / "buffer";
	std::ifstream file;
	std::vector<std::string> bufferNameList;
};

TEST_F(CommandBufferMangerTS, TC1)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillRepeatedly(testing::Return(CommandInfo{ 0, 1, 4 }));

	ssd->run(dummyArgc, dummyArgv);

	for (const auto& entry : fs::directory_iterator(folderPath))
	{
		std::string filename = entry.path().filename().string();
		bufferNameList.push_back(filename);
	}

	EXPECT_EQ(makeCommandBufferName(0, CommandInfo{0, 1, 4}), bufferNameList[0]);
}

TEST_F(CommandBufferMangerTS, TC2)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillOnce(testing::Return(CommandInfo{ 0, 1, 0xFFFFFFFF }))
		.WillOnce(testing::Return(CommandInfo{ 0, 2, 0x12345678 }))
		.WillOnce(testing::Return(CommandInfo{ 0, 3, 0x00000001 }))
		.WillOnce(testing::Return(CommandInfo{ 0, 4, 0xABCE1234 }))
		.WillOnce(testing::Return(CommandInfo{ 0, 5, 0xAAAAAAAA }));
	
	for (int i = 0; i < 5; i++)
	{
		CommandBufferManager::getInstance().initialize();
		ssd->run(dummyArgc, dummyArgv);
	}

	for (const auto& entry : fs::directory_iterator(folderPath))
	{
		std::string filename = entry.path().filename().string();
		bufferNameList.push_back(filename);
	}

	EXPECT_EQ(makeCommandBufferName(0, CommandInfo{ 0, 1, 0xFFFFFFFF }), bufferNameList[0]);
	EXPECT_EQ(makeCommandBufferName(1, CommandInfo{ 0, 2, 0x12345678 }), bufferNameList[1]);
	EXPECT_EQ(makeCommandBufferName(2, CommandInfo{ 0, 3, 0x00000001 }), bufferNameList[2]);
	EXPECT_EQ(makeCommandBufferName(3, CommandInfo{ 0, 4, 0xABCE1234 }), bufferNameList[3]);
	EXPECT_EQ(makeCommandBufferName(4, CommandInfo{ 0, 5, 0xAAAAAAAA }), bufferNameList[4]);
}

TEST_F(CommandBufferMangerTS, OptimizeCommandBUfferTC1)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillOnce(testing::Return(CommandInfo{ 0, 0, 0x11112222 }))
		.WillOnce(testing::Return(CommandInfo{ 2, 0, 2 }));

	for (int i = 0; i < 2; i++)
	{
		CommandBufferManager::getInstance().initialize();
		ssd->run(dummyArgc, dummyArgv);
	}

	for (const auto& entry : fs::directory_iterator(folderPath))
	{
		std::string filename = entry.path().filename().string();
		bufferNameList.push_back(filename);
	}

	EXPECT_EQ(makeCommandBufferName(0, CommandInfo{ 2, 0, 2 }), bufferNameList[0]);
	EXPECT_EQ(makeCommandBufferName(1, CommandInfo{ 5, 2, 0x12345678 }), bufferNameList[1]);
	EXPECT_EQ(makeCommandBufferName(2, CommandInfo{ 5, 3, 0x00000001 }), bufferNameList[2]);
	EXPECT_EQ(makeCommandBufferName(3, CommandInfo{ 5, 4, 0xABCE1234 }), bufferNameList[3]);
	EXPECT_EQ(makeCommandBufferName(4, CommandInfo{ 5, 5, 0xAAAAAAAA }), bufferNameList[4]);
}

TEST_F(CommandBufferMangerTS, OptimizeCommandBUfferTC2)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillOnce(testing::Return(CommandInfo{ 2, 0, 2 }))
		.WillOnce(testing::Return(CommandInfo{ 0, 0, 0x00001111 }))
		.WillOnce(testing::Return(CommandInfo{ 0, 1, 0x11112222 }));

	for (int i = 0; i < 3; i++)
	{
		CommandBufferManager::getInstance().initialize();
		ssd->run(dummyArgc, dummyArgv);
	}

	for (const auto& entry : fs::directory_iterator(folderPath))
	{
		std::string filename = entry.path().filename().string();
		bufferNameList.push_back(filename);
	}

	EXPECT_EQ(makeCommandBufferName(0, CommandInfo{ 0, 1, 0x11112222 }), bufferNameList[0]);
	EXPECT_EQ(makeCommandBufferName(1, CommandInfo{ 0, 0, 0x00001111 }), bufferNameList[1]);
	EXPECT_EQ(makeCommandBufferName(2, CommandInfo{ 5, 3, 0x00000001 }), bufferNameList[2]);
	EXPECT_EQ(makeCommandBufferName(3, CommandInfo{ 5, 4, 0xABCE1234 }), bufferNameList[3]);
	EXPECT_EQ(makeCommandBufferName(4, CommandInfo{ 5, 5, 0xAAAAAAAA }), bufferNameList[4]);
}

TEST_F(CommandBufferMangerTS, OptimizeCommandBUfferTC3)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillOnce(testing::Return(CommandInfo{ 0, 0, 0x00001111 }))
		.WillOnce(testing::Return(CommandInfo{ 0, 0, 0x88889999 }));

	for (int i = 0; i < 2; i++)
	{
		CommandBufferManager::getInstance().initialize();
		ssd->run(dummyArgc, dummyArgv);
	}

	for (const auto& entry : fs::directory_iterator(folderPath))
	{
		std::string filename = entry.path().filename().string();
		bufferNameList.push_back(filename);
	}

	EXPECT_EQ(makeCommandBufferName(0, CommandInfo{ 0, 0, 0x88889999 }), bufferNameList[0]);
	EXPECT_EQ(makeCommandBufferName(1, CommandInfo{ 5, 1, 0x11112222 }), bufferNameList[1]);
	EXPECT_EQ(makeCommandBufferName(2, CommandInfo{ 5, 3, 0x00000001 }), bufferNameList[2]);
	EXPECT_EQ(makeCommandBufferName(3, CommandInfo{ 5, 4, 0xABCE1234 }), bufferNameList[3]);
	EXPECT_EQ(makeCommandBufferName(4, CommandInfo{ 5, 5, 0xAAAAAAAA }), bufferNameList[4]);
}

TEST_F(CommandBufferMangerTS, OptimizeCommandBUfferTC4)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillOnce(testing::Return(CommandInfo{ 2, 0, 2 }))
		.WillOnce(testing::Return(CommandInfo{ 0, 0, 0x00001111 }));

	for (int i = 0; i < 2; i++)
	{
		CommandBufferManager::getInstance().initialize();
		ssd->run(dummyArgc, dummyArgv);
	}

	for (const auto& entry : fs::directory_iterator(folderPath))
	{
		std::string filename = entry.path().filename().string();
		bufferNameList.push_back(filename);
	}

	EXPECT_EQ(makeCommandBufferName(0, CommandInfo{ 2, 0, 2 }), bufferNameList[0]);
	EXPECT_EQ(makeCommandBufferName(1, CommandInfo{ 0, 0, 0x00001111 }), bufferNameList[1]);
	EXPECT_EQ(makeCommandBufferName(2, CommandInfo{ 5, 3, 0x00000001 }), bufferNameList[2]);
	EXPECT_EQ(makeCommandBufferName(3, CommandInfo{ 5, 4, 0xABCE1234 }), bufferNameList[3]);
	EXPECT_EQ(makeCommandBufferName(4, CommandInfo{ 5, 5, 0xAAAAAAAA }), bufferNameList[4]);
}

TEST_F(CommandBufferMangerTS, OptimizeCommandBUfferTC5)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillOnce(testing::Return(CommandInfo{ 2, 0, 8 }))
		.WillOnce(testing::Return(CommandInfo{ 2, 8, 3 }))	
		.WillOnce(testing::Return(CommandInfo{ 2, 11, 5 }));

	for (int i = 0; i < 3; i++)
	{
		CommandBufferManager::getInstance().initialize();
		ssd->run(dummyArgc, dummyArgv);
	}

	for (const auto& entry : fs::directory_iterator(folderPath))
	{
		std::string filename = entry.path().filename().string();
		bufferNameList.push_back(filename);
	}

	EXPECT_EQ(makeCommandBufferName(0, CommandInfo{ 2, 10, 6 }), bufferNameList[0]);
	EXPECT_EQ(makeCommandBufferName(1, CommandInfo{ 2, 0, 10 }), bufferNameList[1]);
	EXPECT_EQ(makeCommandBufferName(2, CommandInfo{ 5, 3, 0x00000001 }), bufferNameList[2]);
	EXPECT_EQ(makeCommandBufferName(3, CommandInfo{ 5, 4, 0xABCE1234 }), bufferNameList[3]);
	EXPECT_EQ(makeCommandBufferName(4, CommandInfo{ 5, 5, 0xAAAAAAAA }), bufferNameList[4]);
}

TEST_F(CommandBufferMangerTS, OptimizeCommandBUfferTC6)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillOnce(testing::Return(CommandInfo{ 2, 10, 4 }))
		.WillOnce(testing::Return(CommandInfo{ 2, 12, 3 }));

	for (int i = 0; i < 2; i++)
	{
		CommandBufferManager::getInstance().initialize();
		ssd->run(dummyArgc, dummyArgv);
	}

	for (const auto& entry : fs::directory_iterator(folderPath))
	{
		std::string filename = entry.path().filename().string();
		bufferNameList.push_back(filename);
	}

	EXPECT_EQ(makeCommandBufferName(0, CommandInfo{ 2, 10, 5 }), bufferNameList[0]);
	EXPECT_EQ(makeCommandBufferName(1, CommandInfo{ 5, 0, 10 }), bufferNameList[1]);
	EXPECT_EQ(makeCommandBufferName(2, CommandInfo{ 5, 3, 0x00000001 }), bufferNameList[2]);
	EXPECT_EQ(makeCommandBufferName(3, CommandInfo{ 5, 4, 0xABCE1234 }), bufferNameList[3]);
	EXPECT_EQ(makeCommandBufferName(4, CommandInfo{ 5, 5, 0xAAAAAAAA }), bufferNameList[4]);
}