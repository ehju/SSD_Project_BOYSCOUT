#pragma once

#include "command_interface.h"
#include "Write.h"  // Write 클래스 선언 필요

class Erase : public ICommand {
public:
    void execute(unsigned int lba, unsigned int size) override;

private:
    Write write;
};