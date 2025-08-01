#include "gmock/gmock.h"

#include <fstream>
#include <string>
#include <sstream>
#include "write.h"
#include <filesystem>
#include "SSDHelper.h"

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
	SSDHelper ssdHelper;
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

	actual = ssdHelper.directAccessNand(0);
	EXPECT_EQ(ssdHelper.makeExpectedNandString(0, 1), actual);

}

TEST_F(WriteCommandTS, OverWriteTC)
{
	write.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), static_cast<unsigned int>(0x0), static_cast<unsigned int>(0x1) });
	write.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), static_cast<unsigned int>(0x0), static_cast<unsigned int>(0x3) });

	std::string actual;

	actual = ssdHelper.directAccessNand(0);
	EXPECT_EQ(ssdHelper.makeExpectedNandString(0, 3), actual);
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
		actual = ssdHelper.directAccessNand(i);
		EXPECT_EQ(ssdHelper.makeExpectedNandString(i, i + 1), actual);
	}
}
#endif