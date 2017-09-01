#pragma once

class AstNode;

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
};