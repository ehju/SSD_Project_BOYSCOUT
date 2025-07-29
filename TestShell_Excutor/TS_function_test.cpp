#include "gmock/gmock.h"
#include "TS_function.cpp"

using namespace testing;

class MockSSD : public DummySSD {
public:
	MOCK_METHOD(unsigned int, read, (int lba), (override));
	MOCK_METHOD(bool, write, (int lba, unsigned int data), (override));
};

TEST(TS_function, WriteBasic) {
	MockSSD mock;
	TS_function tsf{ &mock };
	int lba = 99;
	unsigned int data = 0x12345678;

	EXPECT_CALL(mock, write(lba, data))
		.Times(1)
		.WillOnce(Return(true));
		

	EXPECT_EQ(true, tsf.write(lba,data));
}

// SSD Read function should be called
TEST(TS_function, ReadMockTest) {
	MockSSD ssd;
	TS_function shell{&ssd};
	int lba = 0;
	EXPECT_CALL(ssd, read(lba))
	    .Times(1);

	unsigned int ret = shell.read(lba);
}

// if invalid LBA range -> throw exception
TEST(TS_function, ReadMockTestLbaException1) {
	MockSSD ssd;
	TS_function shell{ &ssd };
	int lba = -1;

	EXPECT_THROW(shell.read(lba), std::exception);
}
TEST(TS_function, ReadMockTestLbaException2) {
	MockSSD ssd;
	TS_function shell{ &ssd };
	int lba = 100;

	EXPECT_THROW(shell.read(lba), std::exception);
}

// Read Test state test
TEST(TS_function, ReadMockResultTest) {
	MockSSD ssd;
	TS_function shell{ &ssd };
	int lba = 10;

	EXPECT_CALL(ssd, read(lba))
		.Times(1)
		.WillOnce(testing::Return(1000));

	unsigned int ret = shell.read(lba);

	EXPECT_EQ(ret, 1000);
}
TEST(TS_function, NoWriteOutOfRangeLBA1) {
	MockSSD mock;
	TS_function tsf{ &mock };
	int lba = 100;
	unsigned int data = 0x12345678;

	EXPECT_CALL(mock, write(lba, data))
		.Times(0);
	tsf.write(lba, data);
}
TEST(TS_function, NoWriteOutOfRangeLBA2) {
	MockSSD mock;
	TS_function tsf{ &mock };
	int lba = -2;
	unsigned int data = 0x12345678;

	EXPECT_CALL(mock, write(lba, data))
		.Times(0);
	tsf.write(lba, data);
}

