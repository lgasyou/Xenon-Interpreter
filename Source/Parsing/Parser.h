#pragma once

#include "Stable.h"
#include "Parsing/Scanner.h"
#include "AST/AST.h"

class Scope;

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

private:
	Program *newProgram();
	Block *newBlock();
	std::vector<Statement *> parseProgramOrBlock(Scope *scope, Token::Type endFlag);

	// Create new statements 
	Statement *newStatement();
	Statement *newOutStatement();
	Statement *newInStatement();
	Statement *newWhileStatement();
	Statement *newIfStatement();

	// Create new statements 
	VariableProxy *newVariableProxy();
	Literal *newLiteral();

	// Create new expression statements 
	ExpressionStatement *newExpressionStatement(Expression *node);
	Assignment *newAssignment();
	Expression *newCall();

	// Parses declarations in any scope.
	std::vector<Declaration *> newDeclarations();

	// Parses the statement which's type is variable declaration.
	// Like: int a, b, c;
	std::vector<VariableDeclaration *> newVariableDeclarations();
	VariableDeclaration *newVariableDeclaration(VariableProxy *var, const Token &tok);

	// Not available now.
	Declaration *newFunctionDeclaration(VariableProxy *var, const Token &tok);

private:
	// Parse and create expressions.
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
	Scope *current_scope_;
};
