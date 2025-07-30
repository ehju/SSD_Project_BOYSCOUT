#include "gmock/gmock.h"
#include "TS_function.cpp"
#include <vector>

using namespace testing;
using std::vector;

class MockSSD : public DummySSD {
public:
	MOCK_METHOD(unsigned int, read, (int lba), (override));
	MOCK_METHOD(bool, write, (int lba, unsigned int data), (override));
};

class SSDFixture : public Test {
public:
	MockSSD ssd;
	TS_function shell{ &ssd };
	int lba = 0;
	unsigned int data = 0x12345678;
};

TEST_F(SSDFixture, WriteBasic) {

	EXPECT_CALL(ssd, write(lba, data))
		.Times(1)
		.WillOnce(Return(true));
		
	EXPECT_EQ(true, shell.write(lba,data));
}

// SSD Read function should be called
TEST_F(SSDFixture, ReadMockTest) {
	EXPECT_CALL(ssd, read(lba))
	    .Times(1);

	unsigned int ret = shell.read(lba);
}

// if invalid LBA range -> throw exception
TEST_F(SSDFixture, ReadMockTestLbaException1) {
 
	lba = -1;

	EXPECT_THROW(shell.read(lba), std::exception);
}
TEST_F(SSDFixture, ReadMockTestLbaException2) {

	lba = 100;
	EXPECT_THROW(shell.read(lba), std::exception);
}

// Read Test state test
TEST_F(SSDFixture, ReadMockResultTest) {

	lba = 10;

	EXPECT_CALL(ssd, read(lba))
		.Times(1)
		.WillOnce(testing::Return(1000));

	unsigned int ret = shell.read(lba);

	EXPECT_EQ(ret, 1000);
}

TEST_F(SSDFixture, NoWriteOutOfRangeLBA1) {
    lba = 100;

	EXPECT_CALL(ssd, write(lba, data))
		.Times(0);
	shell.write(lba, data);
}

TEST_F(SSDFixture, NoWriteOutOfRangeLBA2) {
    lba = -2;

	EXPECT_CALL(ssd, write(lba, data))
		.Times(0);
	shell.write(lba, data);
}


TEST(TS_function, ReadFullReadTest) {
	MockSSD ssd;
	TS_function shell{ &ssd };
	vector<unsigned int> result;
	EXPECT_CALL(ssd, read(_))
		.Times(100);

	result = shell.fullread();
}

TEST(TS_function, ReadFullReadTestExpectedReturn) {
	MockSSD ssd;
	TS_function shell{ &ssd };
	unsigned int data = 0x00000000;
	
	vector<unsigned int> expected_result;
	vector<unsigned int> result;
	for (int lba = 0; lba < 100; lba++) {
		expected_result.push_back(data);
		EXPECT_CALL(ssd, read(lba))
			.Times(1)
			.WillRepeatedly(Return(data));
		data++;
	}

	result = shell.fullread();
	EXPECT_EQ(expected_result, result);
}