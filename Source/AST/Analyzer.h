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
	void visitStatement(AstNode *node);

	void visitInStatement(InStatement *node);

	void visitOutStatement(OutStatement *node);

	Variable visitVariableProxy(VariableProxy *node);

	AstValue visitLiteral(Literal *literal);
};
