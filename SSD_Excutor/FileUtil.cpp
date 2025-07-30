#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>	

class FileUtil {
public:
	static void deletePrevOutputFile()
	{
		static const std::string OUTPUT_FILE = "ssd_output.txt";
		if (file_exists(OUTPUT_FILE)) {
			if (deleteFile(OUTPUT_FILE) == DELETE_SUCCESS) {
			}
			else {
				std::cout << "fail to delete " << OUTPUT_FILE << std::endl;
			}
		}
	}

	static void writeOutputFile(std::string& writeString)
	{
		static const std::string OUTPUT_FILE = "ssd_output.txt";
		std::ofstream out(OUTPUT_FILE);
		if (out.is_open()) {
			out << writeString;
			out.close();
		}
		else {
			std::cout << "fail to create output file";
		}
	}

private:
	static bool file_exists(const std::string& filename) {
		std::ifstream file(filename);
		return file.good();
	}
	static int deleteFile(const std::string& fileName)
	{
		return std::remove(fileName.c_str());
	}
	static const int DELETE_SUCCESS = 0;
};