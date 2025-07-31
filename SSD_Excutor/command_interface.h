#pragma once
#include <string>
#include "command_parser.h"

class ICommand
{
public:
	virtual void execute(CommandInfo commandInfo) = 0;
};