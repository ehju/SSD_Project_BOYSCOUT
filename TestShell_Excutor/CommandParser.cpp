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

CommandInfo CommandParser::createCommandData(const string cmd)
{
	vector<string> cmdParms = getCommandParams(cmd);
	if (isValidCommand(cmdParms) == false)
		return MakeInvalidCmdData();

	CommandInfo ret = { 0, 0, 0,0 };
	for (CommandFormat cmddata : commandlist)
	{
		if (cmddata.cmd == cmdParms[CMDINDEX])
		{
			ret.command = getCommandType(cmdParms[0]);
			ret.lba = getLBA(cmddata, cmdParms);
			if (cmddata.isUseValue)
				ret.value = getHexValue(cmddata, cmdParms);
			else if (cmddata.isUseEndLBA)
				ret.value = getEndLBA(cmddata, cmdParms);
			else
				ret.value = 0xFFFFFFFF;

			ret.size = getSize(cmddata, cmdParms);
		}
	}
	return ret;
}

unsigned int CommandParser::getCommandType(const string cmd)
{
	auto it = this->cmdMap.find(cmd);
	if (it == this->cmdMap.end())
		return CMD_NOT_SUPPORTED;
	return this->cmdMap[cmd];
}

bool CommandParser::isValidCommand(vector<string> cmdSplits)
{
	if (checkCommand(cmdSplits) == false)
		return false;
	if (checkParamNum(cmdSplits) == false)
		return false;
	if (checkValidLBA(cmdSplits) == false)
		return false;
	if (checkValidValue(cmdSplits) == false)
		return false;
	if (checkValidSize(cmdSplits) == false)
		return false;
	if (checkValiEndLBA(cmdSplits) == false)
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

				return isNumber(lbastr);
			}
			else
				return true;
		}
	}
	return false;
}

bool CommandParser::checkValiEndLBA(vector<string> cmdSplits)
{
	for (CommandFormat cmddata : commandlist)
	{
		if (cmddata.cmd == cmdSplits[CMDINDEX])
		{
			if (cmddata.isUseEndLBA)
			{
				string lbastr = cmdSplits[VALUEINDEX];
				return isNumber(lbastr);
			}
			else
				return true;
		}
	}
	return false;
}

CommandInfo CommandParser::MakeInvalidCmdData()
{
	return CommandInfo{ CMD_NOT_SUPPORTED, 0xFFFFFFFF,0xFFFFFFFF,-1 };
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
				return isHex(valueStr);
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
	else if (type == CMD_BASIC_EXIT) {
		return CMD_BASIC_EXIT;
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
	else if (type == CMD_NOT_SUPPORTED)
		return CMD_NOT_SUPPORTED;
	else {
		return runCommandTestScenario(type);
	}
}

bool CommandParser::runCommandWrite(const string lba, const string value)
{
	int iLba = stoi(lba);
	unsigned int iValue = strtoul(value.c_str(), nullptr, 16);
	bool result = this->shell.write(iLba, iValue);
#ifdef _DEBUG
	std::cout << "[Write] Done\n";
#else
	if (result)
		std::cout << "[Write] Done\n";
#endif
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
	std::cout << "* TEAM_NAME: " << teamName << std::endl;
	std::cout << "* TEAM_LEADER: " << teamLeader << std::endl;
	std::cout << "* TEAM_MEMBER: " << teamMemberName << std::endl;
	for (CommandFormat data : commandlist)
	{
		std::cout << data.cmd << data.usage << std::endl;
	}
}


bool CommandParser::runCommandFullWrite(const string value)
{
	unsigned int iValue = strtoul(value.c_str(), nullptr, 16);
	bool result = this->shell.fullwrite(iValue);
#ifdef _DEBUG
	std::cout << "[Write] Done\n";
#else
	if (result)
		std::cout << "[Write] Done\n";
#endif
	return true;
}

int CommandParser::runCommandFullRead(void)
{
	vector<unsigned int> reads = this->shell.fullread();
	int lba = 0;
	for (auto value : reads) {
		printReadResult(lba++, value);
	}
	return 0;
}

int CommandParser::runCommandTestScenario(int type)
{
	bool result = false;
	if (type == CMD_TS_FullWriteAndReadCompare)
		result = this->shell.fullWriteAndReadCompare();
	else if (type == CMD_TS_PartialLBAWrite)
		result = this->shell.partialLBAWrite();
	else if (type == CMD_TS_WriteReadAging)
		result = this->shell.writeReadAging();

	std::cout << (result ? "PASS" : "FAIL") << std::endl;

	return result;
}

//size is int
bool CommandParser::checkValidSize(vector<string> cmdSplits)
{
	for (CommandFormat cmddata : commandlist)
	{
		if (cmddata.cmd == cmdSplits[CMDINDEX])
		{
			if (cmddata.isUseSize)
			{
				string sizestr = cmdSplits[SIZEINDEX];
				if (sizestr[0] == '-')
				{
					return isNumber(sizestr.substr(1, sizestr.length()));
				}
				else
				{
					return isNumber(sizestr);
				}
			}
			else
				return true;
		}
	}
	return false;
}

bool CommandParser::isNumber(const std::string& str)
{

	for (char ch : str)
	{
		if (ch < '0' || ch > '9')
			return false;

	}
	return true;
}

bool CommandParser::isHex(const std::string& str)
{
	if (str[0] != '0' || str[1] != 'x')
		return false;

	for (int i = HEXSTART; i < str.length(); i++)
	{
		if (str[i] >= '0' && str[i] <= '9')
			continue;
		else if (str[i] >= 'A' && str[i] <= 'F')
			continue;
		else
			return false;
	}
	return true;
}

unsigned int CommandParser::getLBA(const CommandFormat& cmddata, const std::vector<std::string>& cmdSplits)
{
	if (cmddata.isUseLBA)
	{
		return getDecimal(cmdSplits[LBAINDEX]);
	}
	return 0xFFFFFFFF;
}
unsigned int CommandParser::getEndLBA(const CommandFormat& cmddata, const std::vector<std::string>& cmdSplits)
{
	if (cmddata.isUseEndLBA)
	{
		return getDecimal(cmdSplits[VALUEINDEX]);
	}
	return 0xFFFFFFFF;
}
unsigned int CommandParser::getSize(const CommandFormat& cmddata, const std::vector<std::string>& cmdSplits)
{
	if (cmddata.isUseSize)
	{
		return getSignedDecimal(cmdSplits[SIZEINDEX]);
	}
	return -1;
}
unsigned int  CommandParser::getDecimal(const string& str)
{

	try {
		return stoul(str);
	}
	catch (const std::invalid_argument& e) {
		std::cerr << "Invalid argument: " << e.what() << std::endl;
	}
	catch (const std::out_of_range& e) {
		std::cerr << "Out of range: " << e.what() << std::endl;
	}
	return 0xFFFFFFFF;

}

int  CommandParser::getSignedDecimal(const string& str)
{

	try {
		return stol(str);
	}
	catch (const std::invalid_argument& e) {
		std::cerr << "Invalid argument: " << e.what() << std::endl;
	}
	catch (const std::out_of_range& e) {
		std::cerr << "Out of range: " << e.what() << std::endl;
	}
	return 0xFFFFFFFF;

}
unsigned int CommandParser::getHexValue(const CommandFormat& cmddata, const std::vector<std::string>& cmdSplits)
{

	if (cmddata.isUseValue)
	{
		try {

			return std::stoul(cmdSplits[VALUEINDEX], nullptr, 16);
		}
		catch (const std::invalid_argument& e) {
			std::cerr << "Invalid argument: " << e.what() << std::endl;
		}
		catch (const std::out_of_range& e) {
			std::cerr << "Out of range: " << e.what() << std::endl;
		}
	}

	return 0xFFFFFFFF;
}