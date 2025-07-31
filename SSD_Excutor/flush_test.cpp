#include "gmock/gmock.h"
#include "flush.h"
#include "command_buffer_manager.h"
#include <fstream>

namespace fs = std::filesystem;

class FlushTS : public testing::Test {
protected:
	void SetUp() override
	{
		CommandBufferManager::getInstance().clearCommandBuffer();

		fs::create_directory(folderPath);

		file.open(nand);

		if (file.is_open() == false)
		{
			FAIL();
		}

	}

	void TearDown() override
	{
		file.close();
	}

public:
	void makeCommandBuffer(unsigned int index, CommandInfo cmdInfo)
	{
		std::string line = "";
		std::string value = "";

		if (cmdInfo.command == static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE))
		{
			std::ostringstream ss;
			ss << "0x" << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << cmdInfo.value;
			value = ss.str();
			line = std::to_string(index) + "_" + commandParser.getCommandFormat(cmdInfo.command).cmd + "_" + std::to_string(cmdInfo.lba) + "_" + value;
		}
		else if (cmdInfo.command == static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE))
		{
			value = std::to_string(cmdInfo.value);
			line = std::to_string(index) + "_" + commandParser.getCommandFormat(cmdInfo.command).cmd + "_" + std::to_string(cmdInfo.lba) + "_" + value;
		}
		else
		{
			line = std::to_string(index) + "_empty";
		}

		fs::path filePath = folderPath / (line);
		std::ofstream outFile(filePath);
		outFile.close();

	}

	std::string directAccessNand(unsigned int lba)
	{
		std::string line = "";
		file.seekg(0);
		for (int i = 0; i <= lba; i++)
		{
			getline(file, line);
		}
		return line;
	}

	void checkData(unsigned int expectedLba, unsigned int expectedValue, std::string actual)
	{
		std::string expected = "";
		std::ostringstream ss;
		ss << "0x" << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << expectedValue;
		expected = std::to_string(expectedLba) + " " + ss.str();
		EXPECT_EQ(expected, actual);
	}

	CommandParser commandParser;
	fs::path folderPath = fs::current_path() / "buffer";
	std::string nand = "ssd_nand.txt";
	std::ifstream file;

};

TEST_F(FlushTS, TC1)
{
	Flush flush;

	makeCommandBuffer(0, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 1, 1 });
	makeCommandBuffer(1, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 2, 2 });
	makeCommandBuffer(2, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 3, 3 });
	makeCommandBuffer(3, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 4, 4 });
	makeCommandBuffer(4, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 5, 5 });

	CommandBufferManager::getInstance().syncCommandBuffer();

	flush.execute(CommandInfo {});

	std::string actual;
	actual = directAccessNand(1);
	checkData(1, 1, actual);
	actual = directAccessNand(2);
	checkData(2, 2, actual);
	actual = directAccessNand(3);
	checkData(3, 3, actual);
	actual = directAccessNand(4);
	checkData(4, 4, actual);
	actual = directAccessNand(5);
	checkData(5, 5, actual);
	
}

TEST_F(FlushTS, TC2)
{
	Flush flush;

	makeCommandBuffer(0, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 1, 0xFFFF });
	makeCommandBuffer(1, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 2, 0xFFFF });
	makeCommandBuffer(2, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 3, 0x12345678 });
	makeCommandBuffer(3, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 4, 4 });
	makeCommandBuffer(4, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 5, 0xFFFFFFFF});

	CommandBufferManager::getInstance().syncCommandBuffer();

	flush.execute(CommandInfo{});

	std::string actual;
	actual = directAccessNand(1);
	checkData(1, 0xFFFF, actual);
	actual = directAccessNand(3);
	checkData(3, 0x12345678, actual);
	actual = directAccessNand(5);
	checkData(5, 0xFFFFFFFF, actual);

}

TEST_F(FlushTS, TC3)
{
	Flush flush;

	makeCommandBuffer(0, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 1, 0xFFFF });
	makeCommandBuffer(1, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 2, 0xFFFF });
	makeCommandBuffer(2, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 3, 0x12345678 });
	makeCommandBuffer(3, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), 4, 4 });
	makeCommandBuffer(4, CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), 0, 10 });

	CommandBufferManager::getInstance().syncCommandBuffer();

	flush.execute(CommandInfo{});

	std::string actual;
	actual = directAccessNand(1);
	checkData(1, 0, actual);
	actual = directAccessNand(3);
	checkData(3, 0, actual);
	actual = directAccessNand(5);
	checkData(5, 0, actual);

}