#include "Parser.h"

AstNode* Parser::parse() {
	AstNode* node = newStatement();
	return node;
}

void Parser::error() {

}

void Parser::eat(Token::Type tokenType) {
	if (current_token_.type == tokenType) {
		current_token_ = this->scanner_.scan();
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
	VariableNode *promptString = newVariableNode();
	AstNode *node = new OutStatement(0, AstNode::OUT_STATEMENT, promptString);
	return node;
}

VariableNode *Parser::newVariableNode() {
	VariableNode *node = new VariableNode(0, AstNode::VARIABLE, current_token_);
	return node;
}

AstNode *Parser::newInStatement() {
	AstNode *promptString = nullptr;
	AstNode *variable = nullptr;
	if (current_token_.type == Token::STRING_LITERAL) {
		promptString = newVariableNode();
		eat(Token::STRING_LITERAL);
		variable = newIdentifier();
	}
	else {
		variable = newIdentifier();
	}
	AstNode* node = new InStatement(0, AstNode::IN_STATEMENT, promptString, variable);
	return node;
}

AstNode *Parser::newIdentifier() {
	AstNode* node = new Identifier(0, AstNode::VARIABLE, current_token_);
	return node;
}
