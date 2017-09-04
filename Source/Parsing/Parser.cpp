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

Statement *Parser::newStatement() {
	Statement *node = nullptr;

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

std::vector<Declaration *> Parser::newDeclarations() {
	std::vector<Declaration *> nodes;
	Declaration *node = nullptr;
	Token token = current_token_;
	eat(current_token_.type);
	VariableProxy *var = newVariableProxy();
	eat(Token::IDENTIFIER);
	if (current_token_.type == Token::LPAREN)
		node = newFunctionDeclaration();
	else {
		node = newVariableDeclaration(var, token);
		nodes.push_back(node);
		while (current_token_.type != Token::SEMICOLON) {
			eat(Token::COMMA);
			var = newVariableProxy();
			node = newVariableDeclaration(var, token);
			nodes.push_back(node);
			eat(Token::IDENTIFIER);
		}
		eat(Token::SEMICOLON);
	}
	return nodes;
}

Statement *Parser::newOutStatement() {
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

Statement *Parser::newInStatement() {
	VariableProxy *promptString = nullptr;
	VariableProxy *variable = nullptr;
	if (current_token_.type == Token::STRING_LITERAL) {
		promptString = newVariableProxy();
		eat(Token::STRING_LITERAL);
		eat(Token::COMMA);
	}
	variable = newVariableProxy();
	return new InStatement(promptString, variable);
}

VariableProxy *Parser::newVariableProxy() {
	//DBG_PRINT << "VariableNode: " << current_token_.value << "\n";
	return new VariableProxy(current_token_);
}

Literal *Parser::newLiteral() {
	return new Literal(current_token_);
}

AstNode *Parser::newAssignment() {
	VariableProxy *left = newVariableProxy();
	Token token = current_token_;
	eat(Token::ASSIGN);
	Expression *right = expr();
	return new Assignment(token.type, left, right);
}

AstNode *Parser::newBlock() {
	auto type = current_token_.type;
	std::vector<Declaration *> declarations;
	while (type == Token::INT || type == Token::REAL || type == Token::STRING) {
		const auto &d = newDeclarations();
		declarations.insert(declarations.end(), d.begin(), d.end());
	}
	std::vector<Statement *> statements;
	while (current_token_.type != Token::EOS) {
		statements.push_back(newStatement());
	}
	return new Block(declarations, statements);
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

Declaration *Parser::newFunctionDeclaration() {
	//return new FunctionDeclaration();
	return nullptr;
}

Declaration *Parser::newVariableDeclaration(VariableProxy* var, const Token &tok) {
	return new VariableDeclaration(var, tok);
}



#include "Utils/UnitTest.h"
TEST_CASE(TestParserInOut) {
	const std::string source1 = R"(out "Hello, world\n"; out "Test\n"; )";
	const std::string source2 = R"(in "input", a; )";

	const std::string &source = source1;
	Parser parser{ source };
	parser.parse();
}

