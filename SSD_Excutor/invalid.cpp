#include "invalid.h"
#include "FileUtil.h"

void Invalid::execute(CommandInfo commandInfo)
{
	std::string errorStr = "ERROR";
	FileUtil::writeOutputFile(errorStr);
}