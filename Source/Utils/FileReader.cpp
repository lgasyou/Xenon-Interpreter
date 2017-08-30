#include "FileReader.h"

FileReader::FileReader(const std::string fileName)
	: file(fileName) {

}

FileReader::~FileReader() {
	if (file.is_open()) {
		file.close();
	}
}

std::string FileReader::getLine() {
	file.getline(buffer, kBufferSize);
	return std::string(buffer);
}

std::string FileReader::readAll() {
	std::string ret;
	while (!file.eof()) {
		file.getline(buffer, 1000);
		ret.append(buffer);
	}
	return ret;
}
