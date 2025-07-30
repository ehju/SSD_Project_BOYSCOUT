#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>	

class FileUtil {
public:
	void deletePrevOutputFile()
	{
		if (file_exists(OUTPUT_FILE)) {
			if (deleteFile(OUTPUT_FILE) == DELETE_SUCCESS) {
			}
			else {
				std::cout << "fail to delete " << OUTPUT_FILE << std::endl;
			}
		}
	}

private:
	bool file_exists(const std::string& filename) {
		std::ifstream file(filename);
		return file.good();
	}
	int deleteFile(const std::string& fileName)
	{
		return std::remove(fileName.c_str());
	}
	const std::string NAND_FILE = "ssd_nand.txt";
	const std::string OUTPUT_FILE = "ssd_output.txt";
	const int DELETE_SUCCESS = 0;
};