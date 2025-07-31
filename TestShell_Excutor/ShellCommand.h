#pragma once
#include "TS_function.h"

class ShellCommand {
public:
	ShellCommand(iTS_SSD* ssd) : ssd{ ssd } {
		std::srand(RAND_SEED);  // set seed for random		
	}

	bool readCompare(int lba, unsigned int writtenData);

	//TestScenario
	bool fullWriteAndReadCompare();
	bool partialLBAWrite();
	bool writeReadAging();

	unsigned int read(int lba);
	bool write(int lba, unsigned int data);
	vector<unsigned int> fullread();
	bool fullwrite(unsigned int data);
private:
	iTS_SSD* ssd;
	const int RAND_SEED = 1;
	unsigned int getRandomUnsignedInt();
};
