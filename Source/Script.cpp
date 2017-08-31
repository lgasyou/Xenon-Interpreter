#include "Script.h"
#include "Parsing/Parser.h"
#include "AST/Analyzer.h"

Script::Script(AstNode *root) 
	: root_(root) {}

Script Script::Compile(const std::string &source) {
	Parser parser{ source };
	auto tree = parser.parse();
	return Script(tree);
}

void Script::Run() {
	Analyzer analyzer;
	analyzer.visit(root_);
}