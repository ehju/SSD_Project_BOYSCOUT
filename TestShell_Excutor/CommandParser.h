#pragma once
#include <vector>
using std::string;
using std::vector;
class CommandParser {
public:
	vector<string> commandlist = { "write","read","exit","help","fullwrite","fullread" };
	bool InvalidCheck(string str);

};