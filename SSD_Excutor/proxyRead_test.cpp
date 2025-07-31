#include "gmock/gmock.h"
#include "read.h"
#include "write.h"
#include "SSDHelper.h"
#include "command_buffer_manager.h"
#include "proxyRead.h"

using namespace testing;

class MockCommandBufferManager : public CommandBufferManager {
public:
    MOCK_METHOD(std::vector<CommandBufferInfo>, getCommandBufferList,(), (override));
};

class ProxyReadTestFixture : public Test {
public:
    SSDHelper ssdHelper;
    Read readCommand;
    Write writeCommand;
    MockCommandBufferManager mock;
    ProxyRead proxyRead{ &mock };
};

// hit buffer
TEST_F(ProxyReadTestFixture, HitBuffer)
{
    //buffer에서 가져온 값이 hit 되어야 한다.
    std::vector<CommandBufferInfo> stubList;
    unsigned int cmd = 0;
    unsigned int param1 = 0x1;
    unsigned int param2 = 0x2;
    std::shared_ptr<ICommand> stubCmd = std::make_shared<Read>();
    stubList.push_back(CommandBufferInfo{ cmd, param1,param2, stubCmd });

    EXPECT_CALL(mock, getCommandBufferList)
        .WillOnce(Return(stubList));
    EXPECT_EQ(proxyRead.bufferHit(param1), true);
}

// Fail to hit buffer
TEST_F(ProxyReadTestFixture, FailToHitBuffer)
{
    std::vector<CommandBufferInfo> stubList;
    unsigned int cmd = 0;
    unsigned int param1 = 0x1;
    unsigned int param2 = 0x2;
    std::shared_ptr<ICommand> stubCmd = std::make_shared<Read>();
    stubList.push_back(CommandBufferInfo{ cmd, param1,param2, stubCmd });

    EXPECT_EQ(proxyRead.bufferHit((unsigned int)0x0), false);
}

// Read From buffer
TEST_F(ProxyReadTestFixture, ReadFromBuffer)
{
    std::vector<CommandBufferInfo> stubList;
    unsigned int cmd = 0;
    unsigned int param1 = 0x1;
    unsigned int param2 = 0x2;
    std::shared_ptr<ICommand> stubCmd = std::make_shared<Read>();
    stubList.push_back(CommandBufferInfo{ cmd, param1,param2, stubCmd });

    EXPECT_EQ(proxyRead.getHexValueFromBuffer(param1), "0x00000002");
}