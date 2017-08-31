#pragma once

class AstNode;

/* Basic Visitor. */
class AstNodeVisitor {
public:
	virtual ~AstNodeVisitor() {}

	virtual void visit(AstNode *node) = 0;
};


class Analyzer : public AstNodeVisitor {
public:
	void visit(AstNode *node) override;
};