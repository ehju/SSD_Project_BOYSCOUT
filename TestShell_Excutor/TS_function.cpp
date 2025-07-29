#include <iostream>

class DummySSD {
public:
	virtual unsigned int read(int lba)=0;
	virtual bool write(int lba , unsigned int data) = 0;
};

class TS_function {
public:
	TS_function(DummySSD* ssd) : ssd { ssd } {}

	unsigned int read(int lba) {
		if (lba < 0 || lba > 99) {
			throw std::exception("invalid LBA");
		}
		return  ssd->read(lba);
	}

	bool write(int lab, unsigned int data) {
		return 1;
	}

private:
	DummySSD *ssd;
};
