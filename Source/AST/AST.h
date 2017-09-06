#pragma once

#include "Stable.h"
#include <vector>
#include "Parsing/Token.h"
#include "Utils/Zone.h"

class AstValue;
class Variable;
class VariableProxy;
class Literal;
class Expression;
class Declaration;

/* Basic AstNode. */
class AstNode : public ZoneObject {
public:
	enum NodeType {
		/* Declarations */
		VARIABLE_DECLARATION,
		FUNCTION_DECLARATION,

		/* Statements */
		WHILE_STATEMENT,
		BLOCK,
		EXPRESSION_STATEMENT,
		EMPTY_STATEMENT,
		IF_STATEMENT,
		BREAK_STATEMENT,
		RETURN_STATEMENT,
		OUT_STATEMENT,
		IN_STATEMENT,

		/* Expressions */
		ASSIGNMENT,
		CALL,
		UNARY_OPERATION,
		BINARY_OPERATION,
		COMPARE_OPERATION,
		VARIABLE,
		LITERAL,
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
protected:
	Statement(int position, NodeType type)
		: AstNode(position, type) {}
};


class ExpressionStatement final : public Statement {
public:
	ExpressionStatement(Expression *expression, int pos = 0)
		: Statement(pos, EXPRESSION_STATEMENT), expression_(expression) {}

	void setExpression(Expression* e) { expression_ = e; }
	Expression* expression() const { return expression_; }

private:
	Expression* expression_;
};


//class BreakableStatement : public Statement {
//protected:
//	BreakableStatement(ZoneList<const AstRawString*>* labels, int position, NodeType type)
//		: Statement(position, type) {}
//};

class Block final : public Statement {
public:
	Block(const std::vector<Declaration *> &decls, const std::vector<Statement *> &stmts, int position = 0)
		: Statement(position, BLOCK), declarations_(decls), statements_(stmts) {}

	const std::vector<Declaration *> &declarations() const { return declarations_; }
	const std::vector<Statement *> &statements() const { return statements_; }

public:
	std::vector<Declaration *> declarations_;
	std::vector<Statement *> statements_;
};


class WhileStatement final : public Statement {
public:
	WhileStatement(Expression *condition, Block *body, int position = 0)
		: Statement(position, WHILE_STATEMENT), while_condition_(condition), while_body_(body) {}

	Expression *whileCondition() const { return while_condition_; }
	Block *whileBody() const { return while_body_; }

private:
	Expression *while_condition_;
	Block *while_body_;
};


//class EmptyStatement final : public Statement {
//private:
//	EmptyStatement(int position, NodeType type)
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
	OutStatement(Literal *promptString, Expression *repeatTimes, VariableProxy *outVariableProxy, int argNum, int position = 0)
		: Statement(position, OUT_STATEMENT), prompt_string_(promptString), repeat_times_(repeatTimes), out_variable_proxy_(outVariableProxy), arg_num_(argNum) {}

	Literal *promptString() const { return prompt_string_; }
	Expression *repeatTimes() const { return repeat_times_; }
	VariableProxy *outVariableProxy() const { return out_variable_proxy_; }
	int argNum() const { return arg_num_; }

private:
	Literal *prompt_string_ = nullptr;
	Expression *repeat_times_ = nullptr;
	VariableProxy *out_variable_proxy_ = nullptr;
	int arg_num_ = 0;
};


/* Input Statement. */
class InStatement final : public Statement {
public:
	InStatement(Literal *prompt, VariableProxy *variable, int position = 0)
		: Statement(position, IN_STATEMENT), prompt_string_(prompt), proxy_(variable) {}

	Literal *promptString() const { return prompt_string_; }
	VariableProxy *variableProxy() const { return proxy_; }

private:
	Literal *prompt_string_;
	VariableProxy *proxy_;
};


/* Basic Declaration. */
class Declaration : public AstNode {
public:
	VariableProxy *variableProxy() const { return proxy_; }

protected:
	Declaration(VariableProxy* proxy, int pos, NodeType type)
		: AstNode(pos, type), proxy_(proxy) {}

private:
	VariableProxy *proxy_;
};

class VariableDeclaration final : public Declaration {
public:
	VariableDeclaration(VariableProxy* proxy, const Token &token, int pos = 0)
		: Declaration(proxy, pos, VARIABLE_DECLARATION), token_(token) {}

	Token::Type tokenType() const { return token_.type; }

private:
	Token token_;
};

class FunctionDeclaration final : public Declaration {
public:
	FunctionDeclaration(VariableProxy* proxy, const Token & token, int pos = 0)
		: Declaration(proxy, pos, FUNCTION_DECLARATION), token_(token) {}

private:
	Token token_;
};


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


class Assignment final : public Expression {
public:
	Token::Type op() const { return op_; }
	VariableProxy* target() const { return target_; }
	Expression* value() const { return value_; }

	void setTarget(VariableProxy *e) { target_ = e; }
	void setValue(Expression *e) { value_ = e; }

public:
	Assignment(Token::Type op, VariableProxy *target, Expression *value, int pos = 0)
		: Expression(pos, ASSIGNMENT), op_(op), target_(target), value_(value) {}

private:
	Token::Type op_;
	VariableProxy* target_;
	Expression* value_;
};


class Call final : public Expression {
public:
	Expression *expression() const { return expression_; }
	const std::vector<VariableDeclaration *> &arguments() const { return arguments_; }

	void setExpression(Expression* e) { expression_ = e; }

public:
	Call(Expression* expression, const std::vector<VariableDeclaration *> &arguments, int pos = 0)
		: Expression(pos, CALL), expression_(expression), arguments_(arguments) {}

public:
	Expression* expression_;
	std::vector<VariableDeclaration *> arguments_;
};


class BinaryOperation final : public Expression {
public:
	Token::Type op() const { return op_; }
	Expression* left() const { return left_; }
	void setLeft(Expression* e) { left_ = e; }
	Expression* right() const { return right_; }
	void setRight(Expression* e) { right_ = e; }

public:
	BinaryOperation(Token::Type op, Expression *left, Expression *right, int pos = 0)
		: Expression(pos, BINARY_OPERATION), op_(op), left_(left), right_(right) {}

private:
	Token::Type op_;
	Expression* left_;
	Expression* right_;
};


class CompareOperation final : public Expression {
public:
	Token::Type op() const { return op_; }
	Expression* left() const { return left_; }
	void setLeft(Expression* e) { left_ = e; }
	Expression* right() const { return right_; }
	void setRight(Expression* e) { right_ = e; }

public:
	CompareOperation(Token::Type op, Expression* left, Expression* right, int pos = 0)
		: Expression(pos, COMPARE_OPERATION), op_(op), left_(left), right_(right) {}

private:
	Token::Type op_;
	Expression* left_;
	Expression* right_;
};


class UnaryOperation final : public Expression {
public:
	UnaryOperation(Token::Type op, Expression *expr, int position = 0)
		: Expression(position, UNARY_OPERATION), op_(op), expr_(expr) {}

	Token::Type op() const { return op_; }
	Expression *expression() const { return expr_; }

private:
	Token::Type op_;
	Expression *expr_;
};


class VariableProxy final : public Expression {
public:
	VariableProxy(const Token &token, int position = 0)
		: Expression(position, VARIABLE), token_(token) {}

	const Token::Type &tokenType() const { return token_.type; }
	Variable *variable();

private:
	Token token_;
	Variable *variable_ = nullptr;
};


class Literal final : public Expression {
public:
	Literal(const Token &token, int position = 0)
		: Expression(position, LITERAL), token_(token) {}

	AstValue *value();

private:
	Token token_;
	AstValue *value_ = nullptr;
};
