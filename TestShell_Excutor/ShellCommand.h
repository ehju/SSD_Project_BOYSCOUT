#pragma once
#include "TS_function.h"
#include "CommandData.h"


interface ShellCommandItem {
public:
	ShellCommandItem() {}
	ShellCommandItem(iTS_SSD* ssd){}
	virtual bool execute(CommandInfo cmdInfo)=0;
};


class ShellCommand {
public:
	ShellCommand(iTS_SSD* ssd) : ssd{ ssd } {
		std::srand(RAND_SEED);  // set seed for random		
	}

	//TestScenario
	bool fullWriteAndReadCompare();
	bool partialLBAWrite();
	bool writeReadAging();
	bool eraseWriteAging();
	unsigned int read(int lba);
	bool write(int lba, unsigned int data);
	vector<unsigned int> fullread();
	bool fullwrite(unsigned int data);
	bool erase(int lba, int size);
	bool erase_range(int start_lba, int end_lba);
	bool flush();


private:
	iTS_SSD* ssd;
	const int RAND_SEED = 1;
};
