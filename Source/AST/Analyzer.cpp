#include "Analyzer.h"
#include "AST.h"

std::ostream &operator<<(std::ostream &os, const Variable &var) {
	switch (var.type()) {
	case Variable::INTEGER:
		os << var.toInt();

	case Variable::REAL:
		os << var.toReal();

	case Variable::STRING:
		DBG_PRINT << var.toString();
		os << var.toString();
		os << "\n";
	}
	return os;
}

std::ostream &operator<<(std::ostream &os, Variable &var) {
	const Variable &v = var;
	return operator<<(os, v);
}

void Analyzer::visit(AstNode *root) {
	switch (root->nodeType()) {
	case AstNode::OUT_STATEMENT:
		visitOutStatement((OutStatement*)root);
		break;
	case AstNode::IN_STATEMENT:
		visitInStatement(root);
	default:
		break;
	}
}

void Analyzer::visitInStatement(AstNode *node) {

}

void Analyzer::visitOutStatement(OutStatement *node) {
	std::cout << visitVariableNode(node->promptString());
}
Variable Analyzer::visitVariableNode(VariableProxy *node) {
	return node->value();
}
