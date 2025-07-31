#include "erase.h"

void Erase::execute(unsigned int lba, unsigned int size) {
	unsigned int eraseValue = static_cast<unsigned int>(0x0);
	for (unsigned int i = 0;i < size;i++) {
		write.execute(lba + i, eraseValue);
	}
}
