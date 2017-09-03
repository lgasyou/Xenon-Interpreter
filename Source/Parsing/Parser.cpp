#include "Parser.h"

AstNode *Parser::parse() {
	return newBlock();
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
	eat(Token::SEMICOLON);
	return node;
}

AstNode *Parser::newOutStatement() {
	VariableProxy *promptString = newVariableProxy();
	eat(Token::STRING_LITERAL);
	return new OutStatement(promptString);
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
	return new InStatement(promptString, variable);
}

VariableProxy *Parser::newVariableProxy() {
	//DBG_PRINT << "VariableNode: " << current_token_.value << "\n";
	return new VariableProxy(current_token_);
}

Literal *Parser::newLiteral() {
	return new Literal(current_token_);
}

VariableProxy *Parser::newIdentifier() {
	//DBG_PRINT << "Identifier: " << current_token_.value << "\n";
	return new VariableProxy(current_token_);
}

AstNode *Parser::newBlock() {
	Block *block = new Block(0, AstNode::BLOCK);
	while (current_token_.type != Token::EOS) {
		block->child.push_back(newStatement());
	}
	return block;
}


#include "Utils/UnitTest.h"
TEST_CASE(TestParserInOut) {
	const std::string source1 = R"(out "Hello, world\n"; out "Test\n"; )";
	const std::string source2 = R"(in "input", a; )";

	const std::string &source = source1;
	Parser parser{ source };
	parser.parse();
}
