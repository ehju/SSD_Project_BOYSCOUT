#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>	


class Read {
public:
	void execute(unsigned int address) {
		const std::string NAND_FILE = "ssd_nand.txt";
		const std::string OUTPUT_FILE = "ssd_output.txt";

		// 1. 기존 output 파일 존재 시 삭제
		if (file_exists(OUTPUT_FILE)) {
			if (std::remove(OUTPUT_FILE.c_str()) == 0) {
			}
			else {
				std::cout << "파일 삭제 실패\n";
			}
		}

		std::string matchedValue = "0x00000000";
		std::string line;
		std::ifstream nand(NAND_FILE);

		if (!nand.is_open()) {
			std::cout << "파일 열기 실패: " << NAND_FILE << "\n";
		}
		else {
			while (std::getline(nand, line)) {
				unsigned int addr;
				std::string value;
				std::istringstream iss(line);

				if (!(iss >> addr >> value)) {
					continue; // 형식이 잘못된 줄 무시
				}

				if (addr == address) {
					matchedValue = value; // 최신 값으로 덮어쓰기
				}
			}
			nand.close();
		}

		std::ofstream out(OUTPUT_FILE);
		if (out.is_open()) {
			out << matchedValue;
			out.close();
		}
		else {
			std::cout << "결과 파일 생성 실패\n";
		}

		return;
	}
private:
	bool file_exists(const std::string& filename) {
		std::ifstream file(filename);
		return file.good();
	}
};