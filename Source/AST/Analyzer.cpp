#include "Analyzer.h"
#include "AST.h"

void Analyzer::visit(AstNode *root) {
	switch (root->nodeType()) {
	case AstNode::OUT_STATEMENT:
		visitOutStatement((OutStatement*)root);
		break;

	case AstNode::IN_STATEMENT:
		visitInStatement((InStatement*)root);

	default:
		break;
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
