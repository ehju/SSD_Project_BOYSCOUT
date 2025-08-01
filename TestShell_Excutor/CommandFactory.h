#pragma once
#pragma once
#include "ShellRunner.h"

class CommandFactory
{
public:
	ShellCommandItem* MakeCommandItem(unsigned int type, iTS_SSD* ssd);

};