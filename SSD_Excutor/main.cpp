#include "gmock/gmock.h"
#include "ssd.cpp"

#ifdef _DEBUG
int main() {
    ::testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}
#else
int main(int argc, char* argv[])
{
    SSD ssd;
    ssd.run(argc, argv);
}
#endif