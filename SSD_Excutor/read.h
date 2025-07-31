#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>	

#include "command_interface.h"
#include "command_parser.h"
#include "FileUtil.h"

class Read : public ICommand {
public:
    void execute(CommandInfo commandInfo) override;

private:
    std::string getHexValue(unsigned int address);
    const std::string NAND_FILE = "ssd_nand.txt";
};