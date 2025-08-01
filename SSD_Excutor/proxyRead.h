#pragma once
#include "read.h"
#include "FileUtil.h"
#include "command_buffer_manager.h"

class ProxyRead : public ICommand {
public:
    ProxyRead();
    ProxyRead(CommandBufferManager* ptr);
    void execute(CommandInfo commandInfo) override;
    Read realRead;
    bool bufferHit(unsigned int address);
    std::string getHexValueFromBuffer(unsigned int address);
    std::string toHexString(unsigned int value);
    CommandBufferManager* cbm;

private:
	bool isBufferHitWriteCommand(const DetailedCommandInfo& detailedCommandInfo, int address);
    bool isBufferHitEraseCommand(const DetailedCommandInfo& detailedCommandInfo, int address);
};