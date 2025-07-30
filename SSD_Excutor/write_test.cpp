#include "gmock/gmock.h"

#include <fstream>
#include <string>
#include <sstream>
#include "ssd.cpp"

TEST(TS, FirstWriteAndCreateSsdNandTxtTC)
{
	SSD ssd;
	ssd.writeCommand.execute(0, 1);

	std::string filename = "ssd_nand.txt";
	std::ifstream file(filename);

	EXPECT_TRUE(file.is_open());
}

TEST(TS, OneWrite)
{
	SSD ssd;
	ssd.writeCommand.execute(0, 1);

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

TEST(TS, OverWriteTC)
{
	SSD ssd;
	ssd.writeCommand.execute(0, 1);
	ssd.writeCommand.execute(0, 3);


	std::string filename = "ssd_nand.txt";
	std::ifstream file(filename);
	std::string line;

	std::string expected = "0 0x00000003";

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

TEST(TS, FullWriteAndVerifyTC)
{
	SSD ssd;
	for (int i = 0; i < 100; i++)
	{
		ssd.writeCommand.execute(i, i+1);
	}

	std::string filename = "ssd_nand.txt";
	std::ifstream file(filename);
	std::string line;

	std::string expected;

	if (file.is_open())
	{
		for (int i = 0; i < 100; i++)
		{
			std::ostringstream ss;
			ss << "0x" << std::setfill('0') << std::setw(8) << std::hex << i+1;
			expected = std::to_string(i) + " " + ss.str();
			getline(file, line);
			EXPECT_EQ(expected, line);
		}
		file.close();
	}
	else
	{
		FAIL();
	}
}