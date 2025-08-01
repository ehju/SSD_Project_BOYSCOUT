#pragma once
#include "read.h"
#include "FileUtil.h"
#include "command_buffer_manager.h"

class ProxyRead : public ICommand {
public:
    ProxyRead(CommandBufferManager* ptr);
    void execute(CommandInfo commandInfo) override;
    Read realRead;
    bool bufferHit(unsigned int address);
    std::string getHexValueFromBuffer(unsigned int address);

private:
    bool isBufferHitWriteCommand(const CommandInfo& commandInfo, unsigned int address);
    bool isBufferHitEraseCommand(const CommandInfo& commandInfo, unsigned int address);
    std::string getValueIfHitBuffer(unsigned int address);
    std::string toHexString(unsigned int value);
    CommandBufferManager* cbm;
};