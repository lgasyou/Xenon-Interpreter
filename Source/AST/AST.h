#pragma once

/* Basic AstNode. */
class AstNode {
public:
	// This is an example, may be changed later.
	enum NodeType {
		VariableDeclaration,
		FunctionDeclaration,
		WhileStatement,
		Block,
		ExpressionStatement,
		EmptyStatement,
		IfStatement,
		BreakStatement,
		ReturnStatement,
		Assignment,
		Call,
		Conditional,
		UnaryOperation,
		BinaryOperation,
		CompareOperation,
	};

	// TODO: Move this to protected.
	AstNode(int position, NodeType type)
		: position_(position), node_type_(type) {}

	int position() const { return position_; }

	NodeType nodeType() const { return node_type_; }

//protected:
//	AstNode(int position, NodeType type)
//		: position_(position), node_type_(type) {}

private:
	int position_;
	NodeType node_type_;
};


/* Basic Declaration. */
class Declaration : public AstNode {
public:
	Declaration(int position, NodeType type)
		: AstNode(position, type) {}
};


class FunctionDeclaration : public Declaration {
public:
	FunctionDeclaration(int position, NodeType type)
		: Declaration(position, type) {}
};


class VariableDeclaration : public Declaration {
public:
	VariableDeclaration(int position, NodeType type)
		: Declaration(position, type) {}
};


/* Basic Expression. */
class Expression : public AstNode {
public:
	Expression(int position, NodeType type)
		: AstNode(position, type) {}
};


class Assignment : public Expression {
public:
	Assignment(int position, NodeType type)
		: Expression(position, type) {}
};


class Call : public Expression {
public:
	Call(int position, NodeType type)
		: Expression(position, type) {}
};


class BinaryOperation : public Expression {
public:
	BinaryOperation(int position, NodeType type)
		: Expression(position, type) {}
};


class CompareOperation : public Expression {
public:
	CompareOperation(int position, NodeType type)
		: Expression(position, type) {}
};

class UnaryOperation : public Expression {
public:
	UnaryOperation(int position, NodeType type)
		: Expression(position, type) {}
};


class Conditional : public Expression {
public:
	Conditional(int position, NodeType type)
		: Expression(position, type) {}
};

class VariableProxy : public Expression {
public:
	VariableProxy(int position, NodeType type)
		: Expression(position, type) {}
};

/* Basic Statement. */
class Statement : public AstNode {
public:
	Statement(int position, NodeType type)
		: AstNode(position, type) {}
};


class BreakableStatement : public Statement {
public:
	BreakableStatement(int position, NodeType type)
		: Statement(position, type) {}
};


class Block : public BreakableStatement {
public:
	Block(int position, NodeType type)
		: BreakableStatement(position, type) {}
};


class WhileStatement : public BreakableStatement {
public:
	WhileStatement(int position, NodeType type)
		: BreakableStatement(position, type) {}
};


class EmptyStatement : public Statement {
public:
	EmptyStatement(int position, NodeType type)
		: Statement(position, type) {}
};


class ExpressionStatement : public Statement {
public:
	ExpressionStatement(int position, NodeType type)
		: Statement(position, type) {}
};


class IfStatement : public Statement {
public:
	IfStatement(int position, NodeType type)
		: Statement(position, type) {}
};


class JumpStatement : public Statement {
public:
	JumpStatement(int position, NodeType type)
		: Statement(position, type) {}
};


class ReturnStatement : public JumpStatement {
public:
	ReturnStatement(int position, NodeType type)
		: JumpStatement(position, type) {}
};


/* Output Statement. */
class OutStatement : public Statement {
public:
	OutStatement(int position, NodeType type)
		: Statement(position, type) {}
};


/* Input Statement. */
class InStatement : public Statement {
public:
	InStatement(int position, NodeType type)
		: Statement(position, type) {}
};
