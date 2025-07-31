#pragma once
#include <iostream>
#include <vector>
#include "read.h"
#include "write.h"
#include "command_parser.h"
#include "FileUtil.h"
#include "command_factory.h"

class SSD {
public:
    SSD(CommandParser* commandParser);
    void run(int argc, char* argv[]);

private:
    CommandParser* commandParser;
    CommandFactory commandFactory;
    std::vector<std::shared_ptr<ICommand>> commandList;
};