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
	fs::remove(OUTPUT_FILE_NAME);
	fs::remove(SSD_FILE_NAME);
	fs::remove_all(BUFFER_SPACE_NAME);
}

//bool SSDHelper::file_exists(const std::string& filename) {
//	std::ifstream file(filename);
//	return file.good();
//}

std::string SSDHelper::makeExpectedNandString(unsigned int expectedLba, unsigned int expectedValue)
{
	std::string expected = "";
	std::ostringstream ss;
	ss << "0x" << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << expectedValue;
	expected = std::to_string(expectedLba) + " " + ss.str();
	return expected;
}

std::string SSDHelper::directAccessNand(unsigned int lba)
{
	std::ifstream file(SSD_FILE_NAME);
	std::string line = "";
	file.seekg(0);
	for (unsigned int i = 0; i <= lba; i++)
	{
		getline(file, line);
	}
	file.close();
	return line;
}

void SSDHelper::makeCommandBuffer(unsigned int index, CommandInfo cmdInfo)
{
	std::string line = makeCommandBufferName(index, cmdInfo);

	if (fs::exists(BUFFER_SPACE_NAME) == false)
	{
		fs::create_directories(BUFFER_SPACE_NAME);
	}

	fs::path filePath = BUFFER_SPACE_NAME / (line);
	std::ofstream outFile(filePath);
	outFile.close();
}

std::string SSDHelper::makeCommandBufferName(unsigned int index, CommandInfo cmdInfo)
{
	std::string line = "";
	std::string value = "";

	if (cmdInfo.command == static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE))
	{
		std::ostringstream ss;
		ss << "0x" << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << cmdInfo.value;
		value = ss.str();
		line = std::to_string(index) + "_" + "W" + "_" + std::to_string(cmdInfo.lba) + "_" + value;
	}
	else if (cmdInfo.command == static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE))
	{
		value = std::to_string(cmdInfo.value);
		line = std::to_string(index) + "_" + "E" + "_" + std::to_string(cmdInfo.lba) + "_" + value;
	}
	else
	{
		line = std::to_string(index) + "_empty";
	}
	return line;
}

std::string SSDHelper::getBufferName(unsigned int bufferIndex)
{
	std::vector<std::string> bufferNameList;
	for (const auto& entry : fs::directory_iterator(BUFFER_SPACE_NAME))
	{
		std::string filename = entry.path().filename().string();
		bufferNameList.push_back(filename);
	}

	return bufferNameList[bufferIndex];
}
