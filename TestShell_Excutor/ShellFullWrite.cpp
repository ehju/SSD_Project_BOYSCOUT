#include "ShellFullWrite.h"
bool FullWrite::execute(unsigned int num1, unsigned int num2)
{
	unsigned int lba = num1;
	unsigned int data = num2;
	for (int lba = 0; lba < 100; lba++) {
		if (!ssd->write(lba, data)) return false;
	}
	return true;
}
