#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include <fstream>

using std::vector;
using std::queue;
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
		cmd = SSDEXCUTE + " read " + std::to_string(lba);
		int result = std::system(cmd.c_str());
		if (result == 0) {
			throw std::exception("Error System Call read");
		}

		if (file_exists(OUTFILE)) {
			string result_str = getReadResultFromFile(OUTFILE);
			if (result_str == "ERROR") return 0;
			unsigned int readValue = (unsigned int)std::stoul(result_str, nullptr, 16);

			return readValue;
		}
		return 0;
	}
	bool write(int lba, unsigned int data) override {
		cmd = SSDEXCUTE + " write " + std::to_string(lba) + " " + (toHex(lba));
		int result = std::system(cmd.c_str());
		if (result == 0) {
			return false;
		}
		if (file_exists(OUTFILE)) {
			string result_str = getReadResultFromFile(OUTFILE);
			if (result_str == "ERROR") return false;
		}

		return true;
	}

private:
	bool file_exists(const std::string& filename) {
		std::ifstream file(filename);
		return file.good();
	}

	std::string getReadResultFromFile(string filename) {
		std::ifstream file(filename);
		std::string line;
		if (file.is_open())
		{
			getline(file, line);
			return line;
		}
		return "";
	}


	std::string toHex(unsigned int value) {
		std::stringstream ss;
		ss << "0x" << std::hex << std::uppercase << value;
		return ss.str();
	}
	string cmd = "";
	string SSDEXCUTE = "\".\\SSD_Excutor.exe";
	string OUTFILE = "ssd_output.txt";

};

struct WrittenData {
	int lba;
	unsigned int writtenData;
};
class TS_function {
public:
	TS_function(iTS_SSD* ssd) : ssd { ssd } {
		std::srand(RAND_SEED);  // set seed for random		
	}

	bool readCompare(int lba, unsigned int writtenData) { 
		unsigned int readData = ssd->read(lba);
		if (readData == writtenData) {
			return true;
		}
		else {
			return false;
		}
	};
	bool fullWriteAndReadCompare() {
		unsigned int writeData = 0x12345678;
		unsigned int readData = writeData;
		int LBA_MAX = 99;
		int LBA_MIN = 0;
		int curWriteLBA = LBA_MIN;
		int curReadLBA = LBA_MIN;
		bool flag;
		queue <WrittenData> datas;
		WrittenData data;
		
		while (curReadLBA <= LBA_MAX)
		{
			for (int i = 0; i < 5; i++) {
				data.lba = curWriteLBA;
				data.writtenData = writeData;
				if (curWriteLBA > LBA_MAX) break;
				if (!ssd->write(data.lba, data.writtenData)) return false;
				datas.push(data);
				curWriteLBA++;
			}
			while (!datas.empty())
			{
				data = datas.front();

				if (!readCompare(data.lba, data.writtenData)) return false;
				datas.pop();
			}
			if (curWriteLBA > LBA_MAX) break;
		}
		return true;
	}
	bool partialLBAWrite() {
		unsigned int writeData = 0x12345678;
		queue <WrittenData> datas;
		int loopcount = 30;
		for (int i = 0; i < loopcount;i++) {

			if (!ssd->write(4, writeData)) return false;
			if (!ssd->write(0, writeData)) return false;
			if (!ssd->write(3, writeData)) return false;
			if (!ssd->write(1, writeData)) return false;
			if (!ssd->write(2, writeData)) return false;
			
			for (int i = 0; i < 5; i++) {
				if (!readCompare(i, writeData)) return false;
			}
		}
		return true;
	}

	// writeReadAging does not have unittest
	bool writeReadAging() {
		unsigned int randvalue;
		int loopcount = 200;
		for (int i = 0; i < loopcount; i++) {
			randvalue = getRandomUnsignedInt();
			std::cout << i << "\n";
			if (!ssd->write(0, randvalue)) return false;
			if (!ssd->write(99, randvalue)) return false;
			if (!readCompare(0, randvalue)) return false;
			if (!readCompare(99, randvalue)) return false;
		}
		return true;
	}

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
	const int RAND_SEED= 1;
	
	unsigned int getRandomUnsignedInt() {
		unsigned int high = static_cast<unsigned int>(std::rand()) << 16;
		unsigned int low = static_cast<unsigned int>(std::rand()) & 0xFFFF;
		return high | low;
	}
};
