#pragma once
#include <vector>
#include <iostream>
#include <string>
using std::string;
using std::vector;

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

	int getCommandType(const string cmd);
	bool invalidCommandCheck(string str);
	bool checkParamNum(vector<string> str);


};