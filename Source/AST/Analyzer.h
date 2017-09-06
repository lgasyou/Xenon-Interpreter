#pragma once

#include <string>
#include "AstValue.h"
#include "AST.h"

class AstNode;
class OutStatement;
class InStatement;
class VariableProxy;
class Variable;

/* Basic Visitor. */
class AstNodeVisitor {
public:
	virtual ~AstNodeVisitor() {}

	// Visits root and all its children.
	virtual void visit(AstNode *root) = 0;
};


class Analyzer : public AstNodeVisitor {
public:
	void visit(AstNode *root) override;

private:
	void visitDeclaration(Declaration *node);

	void visitStatement(AstNode *node);
	void visitInStatement(InStatement *node);
	void visitOutStatement(OutStatement *node);
	void visitWhileStatement(WhileStatement *node);

	AstValue visitExpressionStatement(ExpressionStatement *node);
	AstValue &visitAssignment(Assignment *node);

	AstValue visitUnaryOperation(UnaryOperation *node);
	AstValue visitCompareOperation(CompareOperation *node);
	AstValue visitBinaryOperation(BinaryOperation *node);

	AstValue toAstValue(Expression *node);
	Variable &visitVariableProxy(VariableProxy *node);
	AstValue &visitLiteral(Literal *literal);
};
