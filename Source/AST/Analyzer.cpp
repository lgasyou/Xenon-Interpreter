#include "Analyzer.h"
#include "AST.h"
#include "Variable.h"
#include <map>

std::map<std::string, AstValue> GLOBAL_SCPOPE;

// A macro used to cast NODE to NODE_TYPE* 
// and call the function which is suitable for the NODE.
// Use like:
//	VISIT(OutStatement, node);
#define VISIT(NODE_TYPE, NODE) visit##NODE_TYPE(static_cast<NODE_TYPE *>(NODE))

void Analyzer::visit(AstNode *root) {
	auto block = static_cast<Block *>(root);
	for (auto s : block->declarations()) {
		visitDeclaration(s);
	}
	for (auto s : block->statements()) {
		visitStatement(s);
	}
}

static AstValue::Type transformFrom(Token::Type type) {
	switch (type) {
	case Token::INT:
		return AstValue::INTEGER;

	case Token::REAL:
		return AstValue::REAL;

	case Token::STRING:
		return AstValue::STRING;

	default:
		UNREACHABLE();
	}
}

void Analyzer::visitDeclaration(Declaration *node) {
	switch (node->nodeType()) {
	case AstNode::VARIABLE_DECLARATION: {
		auto vd = static_cast<VariableDeclaration *>(node);
		auto &name = vd->variableProxy()->variable()->name();
		GLOBAL_SCPOPE[name] = AstValue(transformFrom(vd->tokenType()));
		break;
	}

	case AstNode::FUNCTION_DECLARATION:
		break;

	default:
		UNREACHABLE();
		break;
	}
}

void Analyzer::visitStatement(AstNode *node) {
	switch (node->nodeType()) {
	case AstNode::OUT_STATEMENT:
		VISIT(OutStatement, node);
		break;

	case AstNode::IN_STATEMENT:
		VISIT(InStatement, node);
		break;

	case AstNode::EXPRESSION_STATEMENT:
		VISIT(ExpressionStatement, node);
		break;

	default:
		UNREACHABLE();
		break;
	}
}

void Analyzer::visitInStatement(InStatement *node) {
	if (node->promptString()) {
		std::cout << visitLiteral(node->promptString());
	}

	auto proxy = node->variableProxy();
	auto &name = proxy->variable()->name();
	auto &target = GLOBAL_SCPOPE[name];
	AstValue input{ target.type() };
	std::cin >> input;
	GLOBAL_SCPOPE[name] = input;
}

void Analyzer::visitOutStatement(OutStatement *node) {
	int times = 1;
	AstValue VariableValue;
	std::string timesValue;

	if (node->repeatTimes() != nullptr) {
		switch ((node->repeatTimes())->nodeType()) {
		case AstNode::VARIABLE:
			timesValue = static_cast<VariableProxy*>(node->repeatTimes())->variable()->name();
			times = GLOBAL_SCPOPE[timesValue].toInt();
			break;

		case AstNode::LITERAL:
			times = static_cast<Literal *>(node->repeatTimes())->value()->toInt();
			break;

		default:
			UNREACHABLE();
			break;
		}
	}
	if (node->outVariableProxy() != nullptr) {
		const auto &name = node->outVariableProxy()->variable()->name();
		VariableValue = GLOBAL_SCPOPE[name];
	}
	// ------------------------------
	switch (node->argNum()) {
	case 1:
		if (node->promptString() != nullptr) {
			std::cout << visitLiteral(node->promptString());
		} else {
			std::cout << VariableValue;
		}
		break;
	case 2:
		if (node->repeatTimes() != nullptr) {
			if (node->promptString() != nullptr) {
				for (int i = 0; i != times; ++i) {
					std::cout << visitLiteral(node->promptString());
				}
			} else {
				for (int i = 0; i != times; ++i) {
					std::cout << VariableValue;
				}
			}
		} else {
			std::cout << visitLiteral(node->promptString());
			std::cout << VariableValue;
		}
		break;

	case 3:
		for (int i = 0; i != times; ++i) {
			std::cout << visitLiteral(node->promptString());
		}
		std::cout << VariableValue;
		break;

	default:
		UNREACHABLE();
		break;
	}
}

void Analyzer::visitExpressionStatement(ExpressionStatement *node) {
	auto expression = node->expression();
	switch (expression->nodeType()) {
	case AstNode::ASSIGNMENT:
		VISIT(Assignment, expression);
		break;

	default:
		UNREACHABLE();
		break;
	}
}

AstValue &Analyzer::visitAssignment(Assignment *node) {
	std::string targetName = node->target()->variable()->name();
	if ((node->value())->nodeType() == AstNode::ASSIGNMENT) {
		return GLOBAL_SCPOPE[targetName] = VISIT(Assignment, node->value());
	}
	return GLOBAL_SCPOPE[targetName] = toAstValue(node->value());
}


AstValue Analyzer::visitOperation(Expression *node) {
	switch (node->nodeType()) {
	case AstNode::BINARY_OPERATION:
		return VISIT(BinaryOperation, node);

	case AstNode::UNARY_OPERATION:
		return VISIT(UnaryOperation, node);

	default:
		UNREACHABLE();
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

	case Token::MOD:
		return toAstValue(left) % toAstValue(right);

	case Token::DIV:
		return toAstValue(left) / toAstValue(right);

	default:
		UNREACHABLE();
	}
}

AstValue Analyzer::visitUnaryOperation(UnaryOperation *node) {
	switch (node->op()) {
	case Token::ADD:
		return toAstValue(node->expression());

	case Token::SUB:
		return AstValue(-1) * toAstValue(node->expression());

	default:
		UNREACHABLE();
	}
}

AstValue Analyzer::toAstValue(Expression *node) {
	auto type = node->nodeType();
	if (type == AstNode::BINARY_OPERATION || type == AstNode::UNARY_OPERATION) {
		return visitOperation(node);
	} else if (type == AstNode::VARIABLE) {
		const std::string &name = static_cast<VariableProxy*>(node)->variable()->name();
		return GLOBAL_SCPOPE[name];
	}
	return VISIT(Literal, node);
}

Variable &Analyzer::visitVariableProxy(VariableProxy *node) {
	return *node->variable();
}

AstValue &Analyzer::visitLiteral(Literal *literal) {
	return *literal->value();
}