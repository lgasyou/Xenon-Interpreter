#include "Script.h"
#include "Parsing/Scanner.h"

Script Script::Compile(const std::string &source) {
	Scanner scanner;
	scanner.scan();
	return Script();
}

std::string Script::Run() {
	return std::string();
}