#include "Analyzer.h"
#include "AST.h"
#include "Variable.h"
#include <map>

std::map<std::string, AstValue> GLOBAL_SCPOPE;

template <typename Old, typename New> 
New Cast(Old &src) {
	return static_cast<New>(src);
}

void Analyzer::visit(AstNode *root) {
	auto block = static_cast<Block *>(root);
	for (auto s : block->declarations()) {
		visitDeclaration(s);
	}
	for (auto s : block->statements()) {
		visitStatement(s);
	}
}

void Analyzer::visitDeclaration(Declaration *node) {

}

void Analyzer::visitStatement(AstNode *node) {
	switch (node->nodeType()) {
	case AstNode::OUT_STATEMENT:
		visitOutStatement((OutStatement*)node);
		break;

	case AstNode::IN_STATEMENT:
		visitInStatement((InStatement*)node);
		break;

	case AstNode::EXPRESSION_STATEMENT:
		visitExpressionStatement(static_cast<ExpressionStatement *>(node));
		break;

	default:
		break;
	}
}

void Analyzer::visitInStatement(InStatement *node) {

}

void Analyzer::visitOutStatement(OutStatement *node) {
	if (node->promptString()) {
		if (node->repeatTimes()) {
			if (node->repeatTimes()->nodeType() == AstNode::LITERAL) {
				auto times = static_cast<Literal *>(node->repeatTimes())->value()->toInt();
				for (int i = 0; i != times; ++i) {
					std::cout << visitLiteral(node->promptString());
				}
				//out promptString * repeatTimes
			} else {
				std::cout << visitLiteral(node->promptString());
			}
		}
	} else {
		const auto &name = node->outVariableProxy()->variable()->name();
		std::cout << GLOBAL_SCPOPE[name];
	}
}

void Analyzer::visitExpressionStatement(ExpressionStatement *node) {
	auto expression = node->expression();
	switch (expression->nodeType()) {
	case AstNode::ASSIGNMENT:
		visitAssignment(static_cast<Assignment *>(expression));
		break;

	default:
		break;
	}
}

void Analyzer::visitAssignment(Assignment *node) {
	std::string varName = node->target()->variable()->name();
	// GG
	GLOBAL_SCPOPE[varName] = visitLiteral(static_cast<Literal *>(node->value()));
}

AstValue Analyzer::visitOperation(Expression *node) {
	switch (node->nodeType()) {
	case AstNode::BINARY_OPERATION:
		return visitBinaryOperation((BinaryOperation*)node);

	case AstNode::UNARY_OPERATION:
	default:
		return visitUnaryOperation((UnaryOperation*)node);
	}
}

AstValue Analyzer::visitBinaryOperation(BinaryOperation *node) {
	auto left = node->left();
	auto right = node->right();
	switch (node->op()) {
	case Token::ADD:
		return toAstValue(left) + toAstValue(right);

	case Token::SUB:
		return toAstValue(left) - toAstValue(right);

	case Token::MUL:
		return toAstValue(left) * toAstValue(right);

	case Token::DIV:
	default:
		return toAstValue(left) / toAstValue(right);
	}
}

AstValue Analyzer::visitUnaryOperation(UnaryOperation *node) {
	switch (node->op()) {
	case Token::ADD:
		return toAstValue(node->expression());

	case Token::SUB:
	default:
		return AstValue(-1) * toAstValue(node->expression());
	}
}

AstValue Analyzer::toAstValue(Expression *node) {
	if (node->nodeType() == AstNode::BINARY_OPERATION || node->nodeType() == AstNode::UNARY_OPERATION) {
		return visitOperation(node);
	}
	return visitLiteral((Literal*)node);
}

Variable &Analyzer::visitVariableProxy(VariableProxy *node) {
	return *node->variable();
}

AstValue &Analyzer::visitLiteral(Literal *literal) {
	return *literal->value();
}