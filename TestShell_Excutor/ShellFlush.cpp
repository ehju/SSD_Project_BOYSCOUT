#include "ShellFlush.h"
bool Flush::execute(CommandInfo cmdInfo)
{
	log->print(__FUNCTION__, "called");
	return ssd->flush();
}
