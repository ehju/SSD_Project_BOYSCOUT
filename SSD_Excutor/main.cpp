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
    Write writeCommand;
    Read readCommand;
    CommandParser commandParser;

    SSD ssd(&commandParser, &writeCommand, &readCommand);

    ssd.run(argc, argv);

    return 1;
}
#endif