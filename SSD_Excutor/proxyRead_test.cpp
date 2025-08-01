#include "gmock/gmock.h"
#include "read.h"
#include "write.h"
#include "SSDHelper.h"
#include "command_buffer_manager.h"
#include "proxyRead.h"

using namespace testing;

class MockCommandBufferManager : public CommandBufferManager {
public:
    MOCK_METHOD(std::vector<DetailedCommandInfo>, getCommandBufferList,(), (override));
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
    std::vector<DetailedCommandInfo> stubList;
    unsigned int cmd = 0;
    unsigned int param1 = 0x1;
    unsigned int param2 = 0x2;
    std::shared_ptr<ICommand> stubCmd = std::make_shared<Write>();
    stubList.push_back(DetailedCommandInfo{ CommandInfo{cmd, param1,param2}, stubCmd });

    EXPECT_CALL(mock, getCommandBufferList)
        .WillOnce(Return(stubList));
    EXPECT_EQ(proxyRead.bufferHit(param1), true);
}

// Fail to hit buffer
TEST_F(ProxyReadTestFixture, FailToHitBuffer)
{
    std::vector<DetailedCommandInfo> stubList;
    unsigned int cmd = 0;
    unsigned int param1 = 0x1;
    unsigned int param2 = 0x2;
    std::shared_ptr<ICommand> stubCmd = std::make_shared<Write>();
    stubList.push_back(DetailedCommandInfo{ CommandInfo{cmd, param1,param2}, stubCmd });
    EXPECT_CALL(mock, getCommandBufferList)
        .WillOnce(Return(stubList));
    EXPECT_EQ(proxyRead.bufferHit((unsigned int)0x2), false);
}

// Read From buffer
TEST_F(ProxyReadTestFixture, ReadFromBuffer)
{
    std::vector<DetailedCommandInfo> stubList;
    unsigned int cmd = 0;
    unsigned int param1 = 0x31;
    unsigned int param2 = 0x32;
    std::shared_ptr<ICommand> stubCmd = std::make_shared<Write>();
    stubList.push_back(DetailedCommandInfo{ CommandInfo{ cmd, param1,param2}, stubCmd });

    EXPECT_CALL(mock, getCommandBufferList)
        .WillRepeatedly(Return(stubList));

    EXPECT_EQ(proxyRead.getHexValueFromBuffer(param1), "0x00000032");
    EXPECT_THAT(proxyRead.getHexValueFromBuffer(param1), Ne(""));
}