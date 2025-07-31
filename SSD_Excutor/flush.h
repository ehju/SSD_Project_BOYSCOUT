#pragma once
#include "command_interface.h"

class Flush : public ICommand
{
public:
	void execute(CommandInfo commandInfo) override;
};