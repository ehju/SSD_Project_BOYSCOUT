#include <iostream>
#include <vector>
#include <queue>
using std::vector;
using std::queue;

#define interface struct

interface iTS_SSD {
public:
	virtual unsigned int read(int lba)=0;
	virtual bool write(int lba , unsigned int data) = 0;
};

struct WrittenData {
	int lba;
	unsigned int writtenData;
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
			for (int i = 0; i < 4; i++) {
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
