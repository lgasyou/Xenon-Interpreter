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
	std::cout << visitVariableNode(node->promptString());
}
Variable Analyzer::visitVariableNode(VariableProxy *node) {
	return node->value();
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