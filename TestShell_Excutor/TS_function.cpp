#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using std::vector;
using std::string;

#define interface struct

interface iTS_SSD {
public:
	virtual unsigned int read(int lba)=0;
	virtual bool write(int lba , unsigned int data) = 0;
};

class SSDExecutor : public iTS_SSD {
public:
	unsigned int read(int lba) override {
		cmd = "\".\\SSD_Excutor.exe\" read " + std::to_string(lba);
		int result = std::system(cmd.c_str());
		if (result == 0) {
			throw std::exception("Error System Call read");
		}

		return 0;
	}
	bool write(int lba, unsigned int data) override {
		cmd = "\".\\SSD_Excutor.exe\" write " + std::to_string(lba) + " " + (toHex(lba));
		int result = std::system(cmd.c_str());
		if (result == 0) {
			return false;
		}
		return true;

	}

private:

	std::string toHex(unsigned int value) {
		std::stringstream ss;
		ss << "0x" << std::hex << std::uppercase << value;
		return ss.str();
	}
	string cmd = "";
};

class TS_function {
public:
	TS_function(iTS_SSD* ssd) : ssd { ssd } {}

	bool readCompare(int lba, unsigned int writtenData) { 
		unsigned int readData = ssd->read(lba);
		if (readData == writtenData) {
			return true;
		}
		else {
			return false;
		}
	};
	bool fullWriteAndReadCompare() { return true; }
	bool partialLBAWrite() { return true; }
	bool writeReadAging() { return true; }

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
		for (int lba = 0; lba < 100; lba++) {
			if (!ssd->write(lba, data)) return false;
		}
		return true;
	}

private:
	iTS_SSD *ssd;
};
