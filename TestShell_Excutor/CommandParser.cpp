#include "CommandParser.h"
#include <sstream>
using namespace std;

int CommandParser::runCommand(const string cmd) {
	// Precondition
	vector<string> cmdParms = getCommandParams(cmd);
	if (isValidCommand(cmdParms) == false)
		return CMD_NOT_SUPPORTED;

	int type = getCommandType(cmdParms[0]);
	runSubCommands(cmdParms, type);
	return type;
}

vector<string> CommandParser::getCommandParams(const string& cmd)
{
	stringstream stream(cmd);
	string word;
	vector<string> tokens;

	while (stream >> word) {
		tokens.push_back(word);
	}
	return tokens;
}

int CommandParser::getCommandType(const string cmd)
{
	auto it = this->cmdMap.find(cmd);
	if (it == this->cmdMap.end())
		return CMD_NOT_SUPPORTED;
	return this->cmdMap[cmd];
}

bool CommandParser::isValidCommand(vector<string> cmdSplits)
{
	if (!checkCommand(cmdSplits))
		return false;
	if (!checkParamNum(cmdSplits))
		return false;
	if (!checkValidLBA(cmdSplits))
		return false;
	if (!checkValidValue(cmdSplits))
		return false;
	return true;
}

bool CommandParser::checkCommand(vector<string> cmdSplits)
{
	for (CommandFormat cmddata : commandlist)
	{
		if (cmddata.cmd == cmdSplits[CMDINDEX])
			return true;
	}
	return false;
}

bool CommandParser::checkParamNum(vector<string> cmdSplits)
{

	for (CommandFormat cmddata : commandlist)
	{
		if (cmddata.cmd == cmdSplits[CMDINDEX])
		{
			if (cmddata.paramnum == cmdSplits.size() - 1)
				return true;
			else
				return false;
		}

	}
	return false;
}

bool CommandParser::checkValidLBA(vector<string> cmdSplits)
{
	for (CommandFormat cmddata : commandlist)
	{
		if (cmddata.cmd == cmdSplits[CMDINDEX])
		{
			if (cmddata.isUseLBA)
			{

				string lbastr = cmdSplits[LBAINDEX];
				if (lbastr.size() <= 0 || lbastr.size() > LBAMAXLENGTH)
					return false;

				for (char lbach : lbastr)
				{
					if (lbach < '0' || lbach > '9')
						return false;

				}
				return true;
			}
			else
				return true;
		}
	}
	return false;
}

bool CommandParser::checkValidValue(vector<string> cmdSplits)
{
	for (CommandFormat cmddata : commandlist)
	{
		if (cmddata.cmd == cmdSplits[CMDINDEX])
		{
			if (cmddata.isUseValue)
			{
				int valueIndex = cmddata.paramnum; //value is lastindex
				string valueStr = cmdSplits[valueIndex];
				if (valueStr.size() != VALUELENGTH)
					return false;
				else if (valueStr[0] != '0' || valueStr[1] != 'x')
					return false;
				bool result = true;
				for (int i = VALUESTART; i < VALUELENGTH; i++)
				{
					if (valueStr[i] >= '0' && valueStr[i] <= '9')
						continue;
					else if (valueStr[i] >= 'A' && valueStr[i] <= 'F')
						continue;
					else
						return false;
				}
				return true;
			}
			else
				return true; //not use value string
		}
	}
	return false;
}

int CommandParser::runSubCommands(vector<string> cmdParms, int type)
{
	if (type == CMD_BASIC_WRITE) {
		return runCommandWrite(cmdParms[1], cmdParms[2]);
	}
	else if (type == CMD_BASIC_READ) {
		return runCommandRead(cmdParms[1]);
	}
	else if (type == CMD_BASIC_HELP) {
		runCommandHelp();
		return CMD_BASIC_HELP;
	}
	else if (type == CMD_BASIC_FULLWRITE) {
		return runCommandFullWrite(cmdParms[1]);
	}
	else if (type == CMD_BASIC_FULLREAD) {
		return runCommandFullRead();
	}
	else
		return CMD_NOT_SUPPORTED;
}

bool CommandParser::runCommandWrite(const string lba, const string value)
{
	int iLba = stoi(lba);
	unsigned int iValue = strtoul(value.c_str(), nullptr, 16);
	bool result = this->shell.write(iLba, iValue);
	if (1) // FIXME
		std::cout << "[Write] Done\n";
	return result;
}

int CommandParser::runCommandRead(const string lba)
{
	int iLba = stoi(lba);
	unsigned int iValue = this->shell.read(iLba);
	printReadResult(iLba, iValue);
	return iValue;
}

void CommandParser::printReadResult(int lba, unsigned int value)
{
	std::stringstream ss;
	ss << "0x" << uppercase << hex << setw(8) << setfill('0') << value;
	std::cout << "[Read] LBA " << lba << " : " << ss.str() << std::endl;
}

void CommandParser::runCommandHelp(void)
{
}

bool CommandParser::runCommandFullWrite(const string value)
{
	unsigned int iValue = strtoul(value.c_str(), nullptr, 16);
	bool result = this->shell.fullwrite(iValue);
	if (1) // FIXME
		std::cout << "[Write] Done\n";
	return true;
}

int CommandParser::runCommandFullRead(void)
{
	vector<unsigned int> reads = this->shell.fullread();
	int lba = 0;
	for(auto value : reads) {
		printReadResult(lba++, value);
	}
	return 0;
}