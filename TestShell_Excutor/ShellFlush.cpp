#include "ShellFlush.h"
bool Flush::execute(CommandInfo cmdInfo)
{
	Logger::getInstance()->print(__FUNCTION__, "called");
	return ssd->flush();
}
