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

	case Token::INT:
		eat(Token::INT);
		node = newDeclaration();
		break;

	case Token::REAL:
		eat(Token::REAL);
		node = newDeclaration();
		break;

	case Token::STRING:
		eat(Token::STRING);
		node = newDeclaration();
		break;

	default:
		break;
	}
	eat(Token::SEMICOLON);
	return node;
}

AstNode *Parser::newDeclaration() {
	AstNode *node = nullptr;
	VariableProxy *var = newVariableProxy();
	eat(Token::IDENTIFIER);
	switch (current_token_.type) {
	case Token::SEMICOLON:
		node = newVariableDeclaration(var);
		break;

	case Token::ASSIGN:
		node = newVariableDeclaration(var);
		doit();
		break;

	case Token::LPAREN:
		node = newFunctionDeclaration();
		break;
	}
	return node;
}

AstNode *Parser::newOutStatement() {
	int flag = 0;
	Literal *promptString = nullptr;
	Expression *repeatTimes = nullptr;
	VariableProxy *outVeriableProxy = nullptr;
	VariableProxy *variableProxy = nullptr;
	while (current_token_.type != Token::SEMICOLON) {
		flag++;
		switch (current_token_.type) {
		case Token::IDENTIFIER:
			variableProxy = newVariableProxy();
			eat(Token::IDENTIFIER);
			break;

		case Token::STRING_LITERAL:
			promptString = newLiteral();
			eat(Token::STRING_LITERAL);
			break;

		case Token::INTEGER_LITERAL:
			repeatTimes = newLiteral();
			eat(Token::INTEGER_LITERAL);
			break;

		case Token::COMMA:
			eat(Token::COMMA);
			break;

		default:
			break;
		}
	}
	if (flag == 1) {
		outVeriableProxy = variableProxy;
	} else if (!repeatTimes) {
		repeatTimes = variableProxy;
	}
	return new OutStatement(promptString, repeatTimes, outVeriableProxy);

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

Expression* Parser::factor() {
	Token token = current_token_;
	if (token.type == Token::ADD) {
		eat(Token::ADD);
		return new UnaryOperation(token.type, factor());
	}

	else if (token.type == Token::SUB) {
		eat(Token::SUB);
		return new UnaryOperation(token.type, factor());
	}

	else if (token.type == Token::INTEGER_LITERAL) {
		eat(Token::INTEGER_LITERAL);
		return new Literal(token);
	}

	else if (token.type == Token::REAL_LITERAL) {
		eat(Token::REAL_LITERAL);
		return new Literal(token);
	}

	else if (token.type == Token::LPAREN) {
		eat(Token::LPAREN);
		Expression *node = expr();
		eat(Token::RPAREN);
		return node;
	}

	return nullptr;
}

Expression* Parser::term() {
	Expression *node = factor();
	while (current_token_.type == Token::MUL || current_token_.type == Token::DIV) {
		Token token = current_token_;
		if (current_token_.type == Token::MUL) {
			eat(Token::MUL);
		} else if (current_token_.type == Token::DIV) {
			eat(Token::DIV);
		}
		node = new BinaryOperation(token.type, node, factor());
	}
	return node;
}

Expression* Parser::expr() {
	Expression *node = term();
	while (current_token_.type == Token::ADD || current_token_.type == Token::SUB) {
		Token token = current_token_;
		if (current_token_.type == Token::ADD) {
			eat(Token::ADD);
		} else if (current_token_.type == Token::SUB) {
			eat(Token::SUB);
		}
		node = new BinaryOperation(token.type, node, term());
	}
	return node;
}

AstNode* Parser::doit() {
	AstNode *node = expr();
	return node;
}

AstNode *Parser::newFunctionDeclaration() {
	//return new FunctionDeclaration();
	return nullptr;
}

AstNode *Parser::newVariableDeclaration(VariableProxy* var) {
	return new VariableDeclaration(var, 0);
}


#include "Utils/UnitTest.h"
TEST_CASE(TestParserInOut) {
	const std::string source1 = R"(out "Hello, world\n"; out "Test\n"; )";
	const std::string source2 = R"(in "input", a; )";

	const std::string &source = source1;
	Parser parser{ source };
	parser.parse();
}

