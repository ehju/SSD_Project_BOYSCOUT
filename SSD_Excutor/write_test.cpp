#include "gmock/gmock.h"

#include <fstream>
#include <string>
#include <sstream>
#include "write.h"

class WriteCommandTS : public testing::Test
{
protected:
	void SetUp() override
	{
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
		std::string line = "";
		file.seekg(0);
		for (int i = 0; i <= lba; i++)
		{
			getline(file, line);
		}
		return line;
	}
};

TEST_F(WriteCommandTS, FirstWriteAndCreateSsdNandTxtTC)
{
	write.execute(0, 1);

	EXPECT_TRUE(file.is_open());
}

TEST_F(WriteCommandTS, OneWrite)
{
	write.execute(0, 1);

	std::string actual;

	actual = directAccessNand(0);
	checkData(0, 1, actual);

}

TEST_F(WriteCommandTS, OverWriteTC)
{
	write.execute(0, 1);
	write.execute(0, 3);

	std::string actual;

	actual = directAccessNand(0);
	checkData(0, 3, actual);
}

TEST_F(WriteCommandTS, FullWriteAndVerifyTC)
{
	for (int i = 0; i < 100; i++)
	{
		write.execute(i, i+1);
	}

	std::string actual;

	for (int i = 0; i < 100; i++)
	{
		actual = directAccessNand(i);
		checkData(i, i + 1, actual);
	}
}