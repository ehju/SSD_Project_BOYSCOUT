#include "gmock/gmock.h"

#include <fstream>
#include <string>
#include <sstream>
#include "write.cpp"

class WriteCommandTS : public testing::Test
{
protected:
	void SetUp() override
	{
		file.open(nand);
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
		ss << "0x" << std::setfill('0') << std::setw(8) << std::hex << expectedValue;
		expected = std::to_string(expectedLba) + " " + ss.str();
		EXPECT_EQ(expected, actual);
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

	getline(file, actual);
	checkData(0, 1, actual);

}

TEST_F(WriteCommandTS, OverWriteTC)
{
	write.execute(0, 1);
	write.execute(0, 3);

	std::string actual;

	getline(file, actual);
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
		getline(file, actual);
		checkData(i, i + 1, actual);
	}
}