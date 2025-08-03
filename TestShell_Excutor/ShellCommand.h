#pragma once
#include "TS_function.h"
#include "CommandData.h"


const int LBA_MAX = 99;
const int LBA_MIN = 0;

interface ShellCommandItem {
public:
	ShellCommandItem() {}
	ShellCommandItem(iTS_SSD* ssd){}
	virtual bool execute(CommandInfo cmdInfo)=0;

};