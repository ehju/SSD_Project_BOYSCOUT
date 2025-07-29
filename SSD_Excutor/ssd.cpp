#include <iostream>
#include "read.cpp"
#include "write.cpp"
#include "command_parser.cpp"

class SSD
{
public:

	Write writeCommand;
	Read readCommand;
	CommandParser commandParser;
};