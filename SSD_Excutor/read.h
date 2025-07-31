#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>	

#include "command_interface.h"
#include "FileUtil.h"

class Read : public ICommand {
public:
    void execute(unsigned int address) override;

private:
    std::string getValue(unsigned int address);
    const std::string NAND_FILE = "ssd_nand.txt";
};