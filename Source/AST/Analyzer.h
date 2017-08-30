#pragma once

/* Basic Visitor. */
class AstNodeVisitor {
public:
	virtual ~AstNodeVisitor() {}

	virtual void visit() = 0;
};


class Analyzer : public AstNodeVisitor {
public:
	void visit() override;
};