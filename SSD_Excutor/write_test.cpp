#include "gmock/gmock.h"

#include <fstream>
#include <string>
#include <sstream>
#include "write.h"
#include <filesystem>
#ifdef _DEBUG
namespace fs = std::filesystem;

class WriteCommandTS : public testing::Test
{
protected:
	void SetUp() override
	{
		fs::remove(nand);
	}

	void TearDown() override
	{
		
	}

public:

	Write write;
	std::string nand = "ssd_nand.txt";
	std::ifstream file;

	void checkData(unsigned int expectedLba, unsigned int expectedValue, std::string actual)
	{
		std::string expected = "";
		std::ostringstream ss;
		ss << "0x" << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << expectedValue;
		expected = std::to_string(expectedLba) + " " + ss.str();
		EXPECT_EQ(expected, actual);
	}

	std::string directAccessNand(unsigned int lba)
	{
		file.open(nand);
		std::string line = "";
		file.seekg(0);
		for (unsigned int i = 0; i <= lba; i++)
		{
			getline(file, line);
		}
		file.close();
		return line;
	}
};

TEST_F(WriteCommandTS, FirstWriteAndCreateSsdNandTxtTC)
{
	write.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), static_cast<unsigned int>(0x0), static_cast<unsigned int>(0x1) });
	EXPECT_TRUE(fs::exists(nand));
}

TEST_F(WriteCommandTS, OneWrite)
{
	write.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), static_cast<unsigned int>(0x0), static_cast<unsigned int>(0x1) });

	std::string actual;

	actual = directAccessNand(0);
	checkData(0, 1, actual);

}

TEST_F(WriteCommandTS, OverWriteTC)
{
	write.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), static_cast<unsigned int>(0x0), static_cast<unsigned int>(0x1) });
	write.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), static_cast<unsigned int>(0x0), static_cast<unsigned int>(0x3) });

	std::string actual;

	actual = directAccessNand(0);
	checkData(0, 3, actual);
}

TEST_F(WriteCommandTS, FullWriteAndVerifyTC)
{
	for (int i = 0; i < 100; i++)
	{
		write.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), static_cast<unsigned int>(i), static_cast<unsigned int>(i+1) });

	}

	std::string actual;

	for (int i = 0; i < 100; i++)
	{
		actual = directAccessNand(i);
		checkData(i, i + 1, actual);
	}
}
#endif