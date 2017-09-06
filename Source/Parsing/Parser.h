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
	Statement *newWhileStatement();
	Statement *newIfStatement();

	VariableProxy *newVariableProxy();
	Literal *newLiteral();

	ExpressionStatement *newExpressionStatement(Expression *node);
	Assignment *newAssignment();
	Expression *newCall();

	std::vector<Declaration *> newDeclarations();
	Declaration *newVariableDeclaration(VariableProxy *var, const Token &tok);
	Declaration *newFunctionDeclaration(VariableProxy *var, const Token &tok);

private:
	//Expression *parseRightSideOfExpression();
	Expression *parseFactor();
	Expression *parseExpression();
	Expression *parseOrExpression();
	Expression *parseMulOrDivExpression();
	Expression *parseAddOrSubExpression();
	Expression *parseLessOrGreaterExpression();
	Expression *parseEqOrNeExpression();
	Expression *parseAndExpression();

private:
	Scanner scanner_;
	Token current_token_;
	Token cached_token_;
	bool peeked_ = false;
};
