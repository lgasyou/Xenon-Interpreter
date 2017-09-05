#include "FileReader.h"

FileReader::FileReader(const std::string &fileName)
	: file_(fileName) {}

FileReader::~FileReader() {
	if (file_.is_open()) {
		file_.close();
	}
}

std::string FileReader::readLine() {
	file_.getline(buffer_, kBufferSize);
	return std::string(buffer_);
}

std::string FileReader::readAll() {
	std::string ret;
	if (!file_.is_open()) {
		return ret;
	}
	while (!file_.eof()) {
		file_.getline(buffer_, 1000);
		ret.append(buffer_);
		ret.push_back('\n');
	}
	return ret;
}


#include "Utils/UnitTest.h"
TEST_CASE(FileReader) {
	const char *fileName = "TestCases/file_reader.test";
	DBG_PRINT << "fileName: " << fileName << '\n';

	FileReader reader{ fileName };
	std::string source = reader.readAll();
	DBG_PRINT << "source:\n" << source;
	ASSERT_NE(0, source.size());
}
