
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip> 

#define interface struct

using std::vector;
using std::queue;
using std::string;


interface iTS_SSD{
public:
	virtual unsigned int read(int lba) = 0;
	virtual bool write(int lba , unsigned int data) = 0;
	virtual bool erase(int lba, int size) = 0;
	virtual bool flush() = 0;
};


class SSDExecutor : public iTS_SSD {
public:
	unsigned int read(int lba) override;
	bool write(int lba, unsigned int data) override;
	bool erase(int lba, int size) override;
	bool flush() override;

private:
	bool file_exists(const std::string& filename);

	std::string getReadResultFromFile(string filename);

	std::string toHex(unsigned int value);
	string cmd = "";
#if _DEBUG
	string SSDEXCUTE = "\".\\SSD_Excutor.exe";
#else
	string SSDEXCUTE = "SSD_Excutor.exe";
#endif
	string OUTFILE = "ssd_output.txt";

};

struct WrittenData {
	int lba;
	unsigned int writtenData;
};


