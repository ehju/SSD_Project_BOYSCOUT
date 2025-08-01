#include "gmock/gmock.h"
#include "flush.h"
#include "command_buffer_manager.h"
#include "SSDHelper.h"
#include <fstream>

#ifdef _DEBUG
namespace fs = std::filesystem;

class FlushTS : public testing::Test {
protected:
	void SetUp() override
	{
		CommandBufferManager::getInstance().initialize();
		ssdHelper.resetSSD();
	}

public:
	CommandParser commandParser;
	SSDHelper ssdHelper;
};

TEST_F(FlushTS, TC1)
{
	Flush flush;

	ssdHelper.makeCommandBuffer(0, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 1, 1 });
	ssdHelper.makeCommandBuffer(1, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 2, 2 });
	ssdHelper.makeCommandBuffer(2, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 3, 3 });
	ssdHelper.makeCommandBuffer(3, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 4, 4 });
	ssdHelper.makeCommandBuffer(4, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 5, 5 });

	CommandBufferManager::getInstance().syncCommandBuffer();

	flush.execute(CommandInfo {});

	for (int i = 1; i <= 5; i++)
	{
		std::string actual;
		actual = ssdHelper.directAccessNand(i);
		EXPECT_EQ(ssdHelper.makeExpectedNandString(i, i), actual);
	}
}

TEST_F(FlushTS, TC2)
{
	Flush flush;

	ssdHelper.makeCommandBuffer(0, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 1, 0xFFFF });
	ssdHelper.makeCommandBuffer(1, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 2, 0xFFFF });
	ssdHelper.makeCommandBuffer(2, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 3, 0x12345678 });
	ssdHelper.makeCommandBuffer(3, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 4, 4 });
	ssdHelper.makeCommandBuffer(4, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 5, 0xFFFFFFFF});

	CommandBufferManager::getInstance().syncCommandBuffer();

	flush.execute(CommandInfo{});

	std::string actual;
	actual = ssdHelper.directAccessNand(1);
	EXPECT_EQ(ssdHelper.makeExpectedNandString(1, 0xFFFF), actual);

	actual = ssdHelper.directAccessNand(3);
	EXPECT_EQ(ssdHelper.makeExpectedNandString(3, 0x12345678), actual);

	actual = ssdHelper.directAccessNand(5);
	EXPECT_EQ(ssdHelper.makeExpectedNandString(5, 0xFFFFFFFF), actual);
}

TEST_F(FlushTS, TC3)
{
	Flush flush;

	ssdHelper.makeCommandBuffer(0, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 1, 0xFFFF });
	ssdHelper.makeCommandBuffer(1, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 2, 0xFFFF });
	ssdHelper.makeCommandBuffer(2, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 3, 0x12345678 });
	ssdHelper.makeCommandBuffer(3, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 4, 4 });
	ssdHelper.makeCommandBuffer(4, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 0, 10 });

	CommandBufferManager::getInstance().syncCommandBuffer();

	flush.execute(CommandInfo{});

	std::string actual;
	actual = ssdHelper.directAccessNand(1);
	EXPECT_EQ(ssdHelper.makeExpectedNandString(1, 0), actual);

	actual = ssdHelper.directAccessNand(3);
	EXPECT_EQ(ssdHelper.makeExpectedNandString(3, 0), actual);

	actual = ssdHelper.directAccessNand(5);
	EXPECT_EQ(ssdHelper.makeExpectedNandString(5, 0), actual);
}
#endif