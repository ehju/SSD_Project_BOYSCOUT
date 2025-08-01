#pragma once
#include "TS_function.h"
#include "CommandData.h"


interface ShellCommandItem {
public:
	ShellCommandItem() {}
	ShellCommandItem(iTS_SSD* ssd){}
	virtual bool execute(CommandInfo cmdInfo)=0;
};