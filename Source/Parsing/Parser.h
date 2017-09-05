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

private:
	void eat(Token::Type tokenType);
	const Token &peek();

	Block *newBlock();

	Statement *newStatement();
	Statement *newOutStatement();
	Statement *newInStatement();

	VariableProxy *newVariableProxy();
	Literal *newLiteral();

	ExpressionStatement *newExpressionStatement(Expression *node);
	Assignment *newAssignment();

	std::vector<Declaration *> newDeclarations();
	Declaration *newVariableDeclaration(VariableProxy *var, const Token &tok);
	Declaration *newFunctionDeclaration();

	Expression *parseRightSideOfExpression();
	Expression *factor();
	Expression *term();
	Expression *expr();

private:
	Scanner scanner_;
	Token current_token_;
	Token cached_token_;
	bool peeked_ = false;
};
