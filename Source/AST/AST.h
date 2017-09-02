#pragma once

#include "Stable.h"
#include "Parsing/Token.h"
#include "Variable.h"

class VariableProxy;

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
		VARIABLE,
		OUT_STATEMENT,
		IN_STATEMENT,
		IDENTIFIER,
	};

	int position() const { return position_; }

	NodeType nodeType() const { return node_type_; }

protected:
	AstNode(int position, NodeType type)
		: position_(position), node_type_(type) {}

private:
	int position_;
	NodeType node_type_;
};


/* Basic Statement. */
class Statement : public AstNode {
public:
	// TODO
	//bool IsEmpty() { return AsEmptyStatement() != nullptr; }
	bool IsJump() const;

protected:
	Statement(int position, NodeType type)
		: AstNode(position, type) {}
};


//class BreakableStatement : public Statement {
//protected:
//	BreakableStatement(ZoneList<const AstRawString*>* labels, int position, NodeType type)
//		: Statement(position, type) {}
//};
//
//
//class Block final : public BreakableStatement {
//public:
//	ZoneList<Statement*>* statements() { return &statements_; }
//
//	bool isJump() const {
//		return !statements_.is_empty() && statements_.last()->IsJump()
//			&& labels() == NULL;  // Good enough as an approximation...
//	}
//
//	Scope* scope() const { return scope_; }
//	void setScope(Scope* scope) { scope_ = scope; }
//
//private:
//	Block(Zone* zone, ZoneList<const AstRawString*>* labels, int capacity, int pos)
//		: BreakableStatement(labels, pos, BLOCK),
//		statements_(capacity, zone),
//		scope_(nullptr) {
//	}
//
//private:
//	ZoneList<Statement*> statements_;
//	Scope* scope_;
//};
//
//
//class WhileStatement final : public BreakableStatement {
//private:
//	WhileStatement(int position, NodeType type)
//		: BreakableStatement(position, type) {}
//};
//
//
//class EmptyStatement final : public Statement {
//private:
//	EmptyStatement(int position, NodeType type)
//		: Statement(position, type) {}
//};
//
//
//class ExpressionStatement final : public Statement {
//private:
//	ExpressionStatement(int position, NodeType type)
//		: Statement(position, type) {}
//};
//
//
//class IfStatement final : public Statement {
//private:
//	IfStatement(int position, NodeType type)
//		: Statement(position, type) {}
//};
//
//
//class JumpStatement : public Statement {
//protected:
//	JumpStatement(int position, NodeType type)
//		: Statement(position, type) {}
//};
//
//
//class ReturnStatement final : public JumpStatement {
//private:
//	ReturnStatement(int position, NodeType type)
//		: JumpStatement(position, type) {}
//};


/* Output Statement. */
class OutStatement final : public Statement {
public:
	OutStatement(int position, NodeType type, VariableProxy *promptString) 
		: Statement(position, type), prompt_string_(promptString) {}

	VariableProxy *promptString() const { return prompt_string_; }

private:
	VariableProxy *prompt_string_;
};


/* Input Statement. */
class InStatement final : public Statement {
public:
	InStatement(int position, NodeType type, VariableProxy *prompt, VariableProxy *variable)
		: Statement(position, type), prompt_string_(prompt), variable_(variable) {}

	VariableProxy *promptString() const { return prompt_string_; }
	VariableProxy *variable() const { return variable_; }

private:
	VariableProxy *prompt_string_;
	VariableProxy *variable_;
};


///* Basic Declaration. */
//class Declaration : public AstNode {
//public:
//	VariableProxy* proxy() const { return proxy_; }
//
//protected:
//	Declaration(VariableProxy* proxy, int pos, NodeType type)
//		: AstNode(pos, type), proxy_(proxy), next_(nullptr) {}
//
//private:
//	VariableProxy *proxy_;
//	Declaration* next_;
//};
//
//
//
//class FunctionDeclaration final : public Declaration {
//private:
//	FunctionDeclaration(int position, NodeType type)
//		: Declaration(position, type) {}
//};
//
//
//class VariableDeclaration final : public Declaration {
//private:
//	VariableDeclaration(VariableProxy* proxy, int pos, bool is_nested = false)
//		: Declaration(proxy, pos) {}
//};


/* Basic Expression. */
class Expression : public AstNode {
public:
	// True iff the expression is a literal represented as a integer.
	bool isIntegerLiteral() const;

	// True iff the expression is a literal represented as a real number.
	bool isRealLiteral() const;

	// True iff the expression is a string literal.
	bool isStringLiteral() const;

	// True iff we can prove that the expression is the undefined literal. Note
	// that this also checks for loads of the global "undefined" variable.
	bool isUndefinedLiteral() const;

protected:
	Expression(int position, NodeType type)
		: AstNode(position, type) {}
};
//
//
//class Assignment final : public Expression {
//public:
//	Token::Value op() const { return op_; }
//	Expression* target() const { return target_; }
//	Expression* value() const { return value_; }
//
//	void setTarget(Expression* e) { target_ = e; }
//	void setValue(Expression* e) { value_ = e; }
//
//private:
//	Assignment(NodeType type, Token::Value op, Expression* target, Expression* value, int pos)
//		: Expression(pos, type), op_(op), target_(target), value_(value) {}
//
//private:
//	Token::Value op_;
//	Expression* target_;
//	Expression* value_;
//};
//
//
//class Call final : public Expression {
//public:
//	Expression* expression() const { return expression_; }
//	ZoneList<Expression*>* arguments() const { return arguments_; }
//
//	void setExpression(Expression* e) { expression_ = e; }
//
//private:
//	Call(Expression* expression, ZoneList<Expression*>* arguments, int pos,
//		PossiblyEval possibly_eval)
//		: Expression(pos, CALL),
//		expression_(expression),
//		arguments_(arguments) {}
//
//	Expression* expression_;
//	ZoneList<Expression*>* arguments_;
//};
//
//
//class BinaryOperation final : public Expression {
//public:
//	Token::Value op() const { return op_; }
//	Expression* left() const { return left_; }
//	void setLeft(Expression* e) { left_ = e; }
//	Expression* right() const { return right_; }
//	void setRight(Expression* e) { right_ = e; }
//
//private:
//	BinaryOperation(Token::Value op, Expression* left, Expression* right, int pos)
//		: Expression(pos, BINARY_OPERATOIN), left_(left), right_(right) {}
//
//private:
//	Token::Value op_;
//	Expression* left_;
//	Expression* right_;
//};
//
//
//class CompareOperation final : public Expression {
//private:
//	CompareOperation(int position, NodeType type)
//		: Expression(position, type) {}
//};
//
//class UnaryOperation final : public Expression {
//private:
//	UnaryOperation(int position, NodeType type)
//		: Expression(position, type) {}
//};
//
//
//class Conditional final : public Expression {
//private:
//	Conditional(int position, NodeType type)
//		: Expression(position, type) {}
//};


class VariableProxy : public Expression {
public:
	VariableProxy(int position, NodeType type, const Token &token)
		: Expression(position, type), token_(token) {}

	const Token &token() const { return token_; }
	Variable value() const;

private:
	Token token_;
};


//AstNode *NewAstNode();
//
//Expression *NewExpression();
//
//Identifier *NewIdentifier();
//
//InStatement *NewInStatement();
//
//OutStatement *NewOutStatement();
//
//Statement *NewStatement();
//
//VariableProxy *NewVariableProxy();
