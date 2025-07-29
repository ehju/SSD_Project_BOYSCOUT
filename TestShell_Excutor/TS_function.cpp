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

	bool write(int lba, unsigned int data) {
		if (lba > 99 || lba < 0) return false;
		return ssd->write(lba, data);
	}

	void fullread() {
		unsigned int data;
		for (int i = 0; i < 100;i++) {
			data= ssd->read(i);
		}
	}

private:
	DummySSD *ssd;
};
