#include "ShellFlush.h"
bool Flush::execute(CommandInfo cmdInfo)
{
	return ssd->flush();
}
