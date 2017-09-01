#pragma once

#include "Stable.h"
#include "Parsing/Scanner.h"

class AstNode;

class Parser {
public:
	Parser(const std::string &source);

	// Returns the root of AST.
	AstNode *parse();

private:
	Scanner scanner_;
};