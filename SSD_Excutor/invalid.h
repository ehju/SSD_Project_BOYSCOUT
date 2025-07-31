#pragma once
#include "command_interface.h"

class Invalid : public ICommand {
public:
	void execute(CommandInfo commandInfo) override;
};