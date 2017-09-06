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


#include "Utils/UnitTest.h"
#include "Utils/FileReader.h"
TEST_CASE(TestScript) {
	const char *fileName = "TestCases/script.test";
	DBG_PRINT << "fileName: " << fileName << '\n';

	FileReader reader{ fileName };
	std::string source = reader.readAll();
	DBG_PRINT << "source:\n" << source;

	Script script = Script::Compile(source);
	DBG_PRINT << "result:\n";
	script.Run();
}
