#pragma once
#include "TS_function.h"


class ShellCommandItem {
public:
	ShellCommandItem() {}
	ShellCommandItem(iTS_SSD* ssd) : ssd{ ssd } {}
	virtual bool execute(unsigned int num1, unsigned int num2)=0;
protected:
	iTS_SSD* ssd;
};


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
	bool erase(int lba, int size);
	bool erase_range(int start_lba, int end_lba);
	bool flush();

private:
	iTS_SSD* ssd;
	const int RAND_SEED = 1;
	unsigned int getRandomUnsignedInt();
};
