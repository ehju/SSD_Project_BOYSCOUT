#pragma once
#include <memory>
#include "command_interface.h"
#include "command_parser.h"
#include "read.h"
#include "write.h"
#include "erase.h"
#include "flush.h"
#include "invalid.h"

class CommandFactory {
public:
    std::shared_ptr<ICommand> CreateCommand(int commandNumber);
};