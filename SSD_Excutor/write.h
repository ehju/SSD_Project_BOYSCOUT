#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "command_interface.h"

class Write : public ICommand {
public:
    Write();

    void execute(unsigned int lba, unsigned int value) override;

private:
    static const unsigned int START_LBA = 0;
    static const unsigned int SSD_SIZE_PER_LBA = 100;

    const std::string nand = "ssd_nand.txt";
    unsigned int map[SSD_SIZE_PER_LBA];

    void sync();
    void flush();
};