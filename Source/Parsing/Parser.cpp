#include "Parser.h"

AstNode *Parser::parse() {
	return newStatement();
}

void Parser::error() {

}

void Parser::eat(Token::Type tokenType) {
	if (current_token_.type == tokenType) {
		current_token_ = scanner_.scan();
		return;
	}
	error();
}

AstNode *Parser::newStatement() {
	AstNode *node = nullptr;

	switch (current_token_.type) {
	case Token::OUT:
		eat(Token::OUT);
		node = newOutStatement();
		break;

	case Token::IN:
		eat(Token::IN);
		node = newInStatement();
		break;

	default:
		break;
	}
	return node;
}

AstNode *Parser::newOutStatement() {
	VariableProxy *promptString = newVariableProxy();
	return new OutStatement(0, AstNode::OUT_STATEMENT, promptString);
}

AstNode *Parser::newInStatement() {
	VariableProxy *promptString = nullptr;
	VariableProxy *variable = nullptr;
	if (current_token_.type == Token::STRING_LITERAL) {
		promptString = newVariableProxy();
		eat(Token::STRING_LITERAL);
		eat(Token::COMMA);
	}
	variable = newIdentifier();
	return new InStatement(0, AstNode::IN_STATEMENT, promptString, variable);
}

VariableProxy *Parser::newVariableProxy() {
	//DBG_PRINT << "VariableNode: " << current_token_.value << "\n";
	return new VariableProxy(0, AstNode::VARIABLE, current_token_);
}

VariableProxy *Parser::newIdentifier() {
	//DBG_PRINT << "Identifier: " << current_token_.value << "\n";
	return new VariableProxy(0, AstNode::IDENTIFIER, current_token_);
}


#include "Utils/UnitTest.h"
TEST_CASE(TestParserInOut) {
	const std::string source1 = R"(out "Hello, world"; )";
	const std::string source2 = R"(in "input", a; )";

	const std::string &source = source1;
	Parser parser{ source };
	parser.parse();
}
