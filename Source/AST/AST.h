#pragma once

/* Basic AstNode. */
class AstNode {
public:
	int position() const { return pos; }

protected:


private:
	int pos;

	//AstNode(int position)
	//	: pos(position) {}
};


/* Basic Declaration. */
class Declaration : public AstNode {

};


class FunctionDeclaration : public Declaration {

};


class VariableDeclaration : public Declaration {

};


/* Basic Expression. */
class Expression : public AstNode {

};


class Assignment : public Expression {

};


class BinaryOperation : public Expression {

};


class Call : public Expression {

};


class CompareOperation : public Expression {

};


class Conditional : public Expression {

};


class UnaryOperation : public Expression {

};


/* Basic Statement. */
class Statement : public AstNode {

};


class WhileStatement : public Statement {

};


class EmptyStatement : public Statement {

};


class ExpressionStatement : public Statement {

};


class IfStatement : public Statement {

};


class JumpStatement : public Statement {

};


class ReturnStatement : public JumpStatement {

};