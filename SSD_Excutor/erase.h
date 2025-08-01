#pragma once

#include "command_interface.h"
#include "Write.h"  // Write Ŭ���� ���� �ʿ�

class Erase : public ICommand {
public:
    void execute(CommandInfo commandInfo) override;

private:
    void doErase(unsigned int address);
    Write write;
};