#include "ShellWrite.h"

bool Write::execute(unsigned int num1, unsigned int num2) {
	unsigned int lba = num1;
	unsigned int data = num2;
	if (lba > 99 || lba < 0) return false;
	return ssd->write(lba, data);
}
