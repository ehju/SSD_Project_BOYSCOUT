#include <iostream>
#include <vector>
using std::vector;

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

	vector<unsigned int> fullread() {
		unsigned int data;
		vector<unsigned int> result;
		for (int lba = 0; lba < 100; lba++) {
			data = (ssd->read(lba));
			result.push_back(data);
		}
		return result;
	}

	bool fullwrite(unsigned int data) {
		return 0;
	}

private:
	DummySSD *ssd;
};
