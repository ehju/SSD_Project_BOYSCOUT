#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
using std::string;
using std::vector;

enum CommandType {
	CMD_NOT_SUPPORTED = -1,
	CMD_BASIC_WRITE = 0,
	CMD_BASIC_READ,
	CMD_BASIC_EXIT,
	CMD_BASIC_HELP,
	CMD_BASIC_FULLWRITE,
	CMD_BASIC_FULLREAD,
	CMD_MAX
};

struct CommandFormat
{
	string cmd;
	int paramnum;
	bool isUseLBA;
	bool isUseValue;
};

class CommandParser {
public:
	vector<CommandFormat> commandlist = {
		{"write",2,true,true},
		{"read",1,true,false},
		{ "exit",0,false,false },
		{"help",0,false,false},
		{"fullwrite",1,false,true},
		{"fullread",0,false,false} };

	int runCommand(const string cmd);
	vector<string> getCommandParams(const std::string& cmd);
	int getCommandType(const string cmd);
	bool invalidCommandCheck(string str);
	bool checkParamNum(vector<string> str);
	bool checkValidLBA(vector<string> str);
	bool checkValidValue(vector<string> str);
private:
	const int LBAMAXLENGTH = 2;
	const int VALUESTART = 2;
	const int VALUELENGTH = 10;

	std::unordered_map<string, int> cmdMap = {
		{"write", CMD_BASIC_WRITE },
		{"read", CMD_BASIC_READ },
		{"exit", CMD_BASIC_EXIT },
		{"help", CMD_BASIC_HELP },
		{"fullwrite", CMD_BASIC_FULLWRITE },
		{"fullread", CMD_BASIC_FULLREAD },
	};
};