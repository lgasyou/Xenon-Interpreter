#pragma once

#include <string>

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
	void visitInStatement(InStatement *node);

	void visitOutStatement(OutStatement *node);

	Variable visitVariableNode(VariableProxy *node);
};
