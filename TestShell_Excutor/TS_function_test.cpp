#include "gmock/gmock.h"
#include "TS_function.cpp"

class MockSSD : public DummySSD {
public:
	MOCK_METHOD(unsigned int, read, (int lba), (override));
	MOCK_METHOD(void, write, (int lba, unsigned int data), (override));
};

TEST(TS_function, TC1) {

}