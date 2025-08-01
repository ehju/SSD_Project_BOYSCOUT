#include "gmock/gmock.h"
#include "read.h"
#include "write.h"
#include "erase.h"
#include "SSDHelper.h"
#include "command_buffer_manager.h"
#include "proxyRead.h"

using namespace testing;
#ifdef _DEBUG
class MockCommandBufferManager : public CommandBufferManager {
public:
    MOCK_METHOD(std::vector<CommandInfo>, getCommandBufferList,(), (override));
};

class ProxyReadTestFixture : public Test {
public:
    SSDHelper ssdHelper;
    Read readCommand;
    Write writeCommand;
    NiceMock<MockCommandBufferManager> mock;
    ProxyRead proxyRead{ &mock };
};

// hit buffer
TEST_F(ProxyReadTestFixture, HitBuffer)
{
    //buffer에서 가져온 값이 hit 되어야 한다.
    std::vector<CommandInfo> stubList;
    unsigned int cmd = static_cast<unsigned int>(0);
    unsigned int targetAddress = static_cast<unsigned int>(0x3);
    unsigned int value = static_cast<unsigned int>(0x3);
    stubList.push_back(CommandInfo{cmd, targetAddress,value});

    EXPECT_CALL(mock, getCommandBufferList)
        .WillOnce(Return(stubList));
    EXPECT_EQ(proxyRead.bufferHit(targetAddress), true);
}

TEST_F(ProxyReadTestFixture, HitBufferWriteCommand)
{
    //buffer에서 가져온 값이 hit 되어야 한다.
    std::vector<CommandInfo> stubList;
    unsigned int targetAddress = static_cast<unsigned int>(0x3);
    stubList.push_back(CommandInfo{0, 1, 1});
    stubList.push_back(CommandInfo{0, 2, 2});
    stubList.push_back(CommandInfo{0, targetAddress, 3});
    stubList.push_back(CommandInfo{0, 4, 4});

    EXPECT_CALL(mock, getCommandBufferList)
        .WillRepeatedly(Return(stubList));
    EXPECT_EQ(proxyRead.bufferHit(targetAddress), true);
}

TEST_F(ProxyReadTestFixture, HitBufferEraseCommand)
{
    //buffer에서 가져온 값이 hit 되어야 한다.
    std::vector<CommandInfo> stubList;
    unsigned int eraseStartAddress = static_cast<unsigned int>(0x11);
    unsigned int eraseRange = static_cast<unsigned int>(0x5);
    
    stubList.push_back(CommandInfo{0, 1, 1});
    stubList.push_back(CommandInfo{0, 2, 2});
    stubList.push_back(CommandInfo{2, eraseStartAddress, eraseRange});
    stubList.push_back(CommandInfo{0, 4, 4});

    EXPECT_CALL(mock, getCommandBufferList)
        .WillRepeatedly(Return(stubList));
    unsigned int targetAddress = static_cast<unsigned int>(0x15);
    EXPECT_EQ(proxyRead.bufferHit(targetAddress), true);
}

// Fail to hit buffer
TEST_F(ProxyReadTestFixture, FailToHitBuffer)
{
    std::vector<CommandInfo> stubList;
    unsigned int cmd = static_cast<unsigned int>(0);
    unsigned int nonTargAddress = 0x1;
    unsigned int targetAddress = static_cast<unsigned int>(0x3);
    unsigned int value = static_cast<unsigned int>(0x2);
    stubList.push_back(CommandInfo{cmd, nonTargAddress,value});
    EXPECT_CALL(mock, getCommandBufferList)
        .WillRepeatedly(Return(stubList));
    EXPECT_EQ(proxyRead.bufferHit(targetAddress), false);
}

// Read From buffer
TEST_F(ProxyReadTestFixture, ReadFromBufferWithWriteCommand)
{
    std::vector<CommandInfo> stubList;
    unsigned int cmd = static_cast<unsigned int>(0);
    unsigned int param1 = static_cast<unsigned int>(0x31);
    unsigned int param2 = static_cast<unsigned int>(0x32);
    stubList.push_back(CommandInfo{ cmd, param1,param2});

    EXPECT_CALL(mock, getCommandBufferList)
        .WillRepeatedly(Return(stubList));

    EXPECT_EQ(proxyRead.getHexValueFromBuffer(param1), "0x00000032");
    EXPECT_THAT(proxyRead.getHexValueFromBuffer(param1), Ne(""));
}

TEST_F(ProxyReadTestFixture, ReadFromBufferWithEraseCommand)
{
    std::vector<CommandInfo> stubList;
    unsigned int cmd = 2;
    unsigned int targetAddress = 0x31;
    unsigned int targetRange = 0x10;
    stubList.push_back(CommandInfo{ cmd, targetAddress,targetRange});

    EXPECT_CALL(mock, getCommandBufferList)
        .WillRepeatedly(Return(stubList));

    unsigned int edgeAddress = targetAddress + targetRange - 1;
    EXPECT_EQ(proxyRead.getHexValueFromBuffer(edgeAddress), "0x00000000");
    EXPECT_THAT(proxyRead.getHexValueFromBuffer(edgeAddress), Ne(""));
}

TEST_F(ProxyReadTestFixture, ReadFromBufferWithComplexCommand)
{
    std::vector<CommandInfo> stubList;
    stubList.push_back(CommandInfo{ 0, 1, 1});
    stubList.push_back(CommandInfo{ 0, 2, 2});
    stubList.push_back(CommandInfo{ 0, 3, 3});
    stubList.push_back(CommandInfo{ 2, 2, 2});

    EXPECT_CALL(mock, getCommandBufferList)
        .WillRepeatedly(Return(stubList));

    EXPECT_EQ(proxyRead.getHexValueFromBuffer(2), "0x00000000");
    EXPECT_EQ(proxyRead.getHexValueFromBuffer(3), "0x00000000");
    EXPECT_THAT(proxyRead.getHexValueFromBuffer(1), Ne("0x00000000"));
    
}
#endif