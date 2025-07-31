#include "SSDHelper.h"

bool SSDHelper::isFileExist(const std::string& fileName) {
	std::ifstream file(fileName);
	return file.is_open();
}
std::string SSDHelper::getReadResultFromFile() {
	std::ifstream file(OUTPUT_FILE_NAME);
	std::string line;
	if (file.is_open())
	{
		getline(file, line);
		return line;
	}
	return "";
}

void SSDHelper::resetSSD() {
	// clear SSD_FILE_
	if (file_exists(SSD_FILE_NAME)) {
		if (std::remove(SSD_FILE_NAME.c_str()) == 0) {
		}
		else {
			std::cout << "fail to delete " << SSD_FILE_NAME << std::endl;
		}
	}
	// clear OUTPUT_FILE_
	if (file_exists(OUTPUT_FILE_NAME)) {
		if (std::remove(OUTPUT_FILE_NAME.c_str()) == 0) {
		}
		else {
			std::cout << "fail to delete " << OUTPUT_FILE_NAME << std::endl;

		}
	}
}

bool SSDHelper::file_exists(const std::string& filename) {
	std::ifstream file(filename);
	return file.good();
}
