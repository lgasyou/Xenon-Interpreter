#include "Analyzer.h"
#include "AST.h"

void Analyzer::visit(AstNode *root) {
	for (auto s : ((Block*)root)->child) {
		visitStatement(s);
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
				// variable;
			}
		} else {
			std::cout << visitLiteral(node->promptString());
		}
	}
}

Variable &Analyzer::visitVariableProxy(VariableProxy *node) {
	return *node->variable();
}

AstValue &Analyzer::visitLiteral(Literal *literal) {
	return *literal->value();
}

void Analyzer::visitStatement(AstNode *node) {
	switch (node->nodeType()) {
	case AstNode::OUT_STATEMENT:
		visitOutStatement((OutStatement*)node);
		break;

	case AstNode::IN_STATEMENT:
		visitInStatement((InStatement*)node);

	default:
		break;
	}
}