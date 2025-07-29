#include "gmock/gmock.h"
#include "TS_function.cpp"

class MockSSD : public DummySSD {
public:
	MOCK_METHOD(unsigned int, read, (int lba), (override));
	MOCK_METHOD(void, write, (int lba, unsigned int data), (override));
};

TEST(TS_function, TC1) {

}


// SSD Read function should be called
TEST(TS_function, ReadMockTest) {
	MockSSD ssd;
	TS_function shell{&ssd};
	int lba = 0;
	EXPECT_CALL(ssd, read(lba))
	    .Times(1);
	
	shell.read(lba);
}
