#pragma once

#include "command_interface.h"
#include "Write.h"  // Write Ŭ���� ���� �ʿ�

class Erase : public ICommand {
public:
    void execute(unsigned int lba, unsigned int size) override;

private:
    Write write;
};