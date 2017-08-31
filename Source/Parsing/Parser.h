#pragma once

#include "Stable.h"
#include "Parsing/Scanner.h"

class AstNode;

class Parser {
public:
	Parser(const std::string &source);

	AstNode *parse();

private:
	Scanner scanner_;
};