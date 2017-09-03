#pragma once

#include "Stable.h"
#include "Parsing/Scanner.h"
#include "AST/AST.h"

class AstNode;

class Parser {
public:
	Parser(const std::string &source)
		: scanner_(source),
		  current_token_(scanner_.scan()) {}

	// Returns the root of AST.
	AstNode *parse();

	// TODO
	void error();

private:
	void eat(Token::Type tokenType);

	AstNode *newStatement();

	AstNode *newOutStatement();

	AstNode *newInStatement();

	VariableProxy *newIdentifier();

	VariableProxy *newVariableProxy();

	Literal *newLiteral();

	AstNode *newBlock();

private:
	Expression *factor();
	Expression *term();
	Expression *expr();
	AstNode *doit();

private:
	Scanner scanner_;
	Token current_token_;
};
