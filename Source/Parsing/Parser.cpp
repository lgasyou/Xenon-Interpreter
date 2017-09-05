#include "Parser.h"

AstNode *Parser::parse() {
	return newBlock();
}

void Parser::eat(Token::Type tokenType) {
	if (peeked_) {
		peeked_ = false;
		current_token_ = cached_token_;
		return;
	}
	if (current_token_.type == tokenType) {
		current_token_ = scanner_.scan();
		return;
	}
	DBG_PRINT << "tokenType: " << Token::Name(tokenType) << " " 
		<< "current_token.type: " << Token::Name(current_token_.type) << "\n";
	UNREACHABLE();
}

const Token &Parser::peek() {
	if (!peeked_) {
		peeked_ = true;
		cached_token_ = scanner_.scan();
	}
	return cached_token_;
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

	case Token::IDENTIFIER:
		node = newExpressionStatement(newAssignment());
		break;

	default:
		UNREACHABLE();
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
	if (current_token_.type == Token::LPAREN) {
		node = newFunctionDeclaration();
	} else {
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
	int repeatAddString = 0, argNum = 0;
	Literal *promptString = nullptr;
	Expression *repeatTimes = nullptr;
	std::vector<VariableProxy*> variableProxies;
	VariableProxy *outVeriableProxy = nullptr;

	while (current_token_.type != Token::SEMICOLON) {
		++argNum;
		switch (current_token_.type) {
		case Token::IDENTIFIER:
			variableProxies.push_back(newVariableProxy());
			eat(Token::IDENTIFIER);
			if (current_token_.type != Token::SEMICOLON) {
				repeatAddString++;
			}
			break;

		case Token::STRING_LITERAL:
			promptString = newLiteral();
			eat(Token::STRING_LITERAL);
			repeatAddString++;
			break;

		case Token::INTEGER_LITERAL:
			repeatTimes = newLiteral();
			eat(Token::INTEGER_LITERAL);
			break;

		case Token::COMMA:
			--argNum;
			eat(Token::COMMA);
			break;

		default:
			UNREACHABLE();
		}
	}

	switch (argNum) {
	case 1:
		if (promptString == nullptr) {
			outVeriableProxy = variableProxies[0];
		}
		break;

	case 2:
		if (variableProxies.size() == 2) {
			repeatTimes = variableProxies[0];
			outVeriableProxy = variableProxies[1];
		}
		if (variableProxies.size() == 1) {
			if (repeatAddString == 2) {
				repeatTimes = variableProxies[0];
			} else {
				outVeriableProxy = variableProxies[0];
			}
		}
		break;

	case 3:
		repeatTimes = variableProxies[0];
		outVeriableProxy = variableProxies[1];
		break;

	default:
		UNREACHABLE();
	}

	return new OutStatement(promptString, repeatTimes, outVeriableProxy, argNum);
}

Statement *Parser::newInStatement() {
	Literal *promptString = nullptr;
	VariableProxy *variable = nullptr;
	if (current_token_.type == Token::STRING_LITERAL) {
		promptString = newLiteral();
		eat(Token::STRING_LITERAL);
		eat(Token::COMMA);
	}
	variable = newVariableProxy();
	eat(Token::IDENTIFIER);
	return new InStatement(promptString, variable);
}

VariableProxy *Parser::newVariableProxy() {
	return new VariableProxy(current_token_);
}

Literal *Parser::newLiteral() {
	return new Literal(current_token_);
}

ExpressionStatement *Parser::newExpressionStatement(Expression *node) {
	return new ExpressionStatement(node);
}

Assignment *Parser::newAssignment() {
	VariableProxy *left = newVariableProxy();
	eat(Token::IDENTIFIER);
	Token token = current_token_;
	eat(Token::ASSIGN);
	Expression *right = parseRightSideOfExpression();
	return new Assignment(token.type, left, right);
}

static bool IsDeclarationStart(Token::Type type) {
	return type == Token::INT || type == Token::REAL || type == Token::STRING;
}

Block *Parser::newBlock() {
	std::vector<Declaration *> declarations;
	std::vector<Statement *> statements;
	auto &type = current_token_.type;
	while (type != Token::EOS) {
		if (IsDeclarationStart(type)) {
			const auto &d = newDeclarations();
			declarations.insert(declarations.end(), d.begin(), d.end());
		} else {
			statements.push_back(newStatement());
		}
	}
	return new Block(declarations, statements);
}

Declaration *Parser::newFunctionDeclaration() {
	//return new FunctionDeclaration();
	return nullptr;
}

Declaration *Parser::newVariableDeclaration(VariableProxy *var, const Token &tok) {
	return new VariableDeclaration(var, tok);
}

Expression* Parser::parseRightSideOfExpression() {
	if (current_token_.type == Token::IDENTIFIER) {
		VariableProxy *left = newVariableProxy();
		if (peek().type == Token::ASSIGN) {
			eat(Token::IDENTIFIER);
			eat(Token::ASSIGN);
			Expression *right = parseRightSideOfExpression();
			return new Assignment(current_token_.type, left, right);
		}
	}
	return expr();
}

Expression *Parser::factor() {
	Token token = current_token_;
	switch (token.type) {
	case Token::ADD:
	case Token::SUB:
	case Token::NOT:
		eat(token.type);
		return new UnaryOperation(token.type, factor());

	case Token::INTEGER_LITERAL:
	case Token::REAL_LITERAL:
	case Token::STRING_LITERAL:
		eat(token.type);
		return new Literal(token);

	case Token::IDENTIFIER:
		eat(Token::IDENTIFIER);
		return new VariableProxy(token);

	case Token::LPAREN:
	{
		eat(Token::LPAREN);
		Expression *node = expr();
		eat(Token::RPAREN);
		return node;
	}

	default:
		UNREACHABLE();
		break;
	}
}

Expression *Parser::mulOrDiv() {
	Expression *node = factor();
	while (FirstIsOneOf(current_token_.type, Token::MUL, Token::DIV, Token::MOD)) {
		Token token = current_token_;
		eat(token.type);
		node = new BinaryOperation(token.type, node, factor());
	}
	return node;
}

Expression *Parser::addOrSub() {
	Expression *node = mulOrDiv();
	while (FirstIsOneOf(current_token_.type, Token::ADD, Token::SUB)) {
		Token token = current_token_;
		eat(token.type);
		node = new BinaryOperation(token.type, node, expr());
	}
	return node;
}

Expression *Parser::largeOrSmall() {
	Expression *node = addOrSub();
	while (FirstIsOneOf(current_token_.type, Token::LT, Token::GT, Token::LTE, Token::GTE)) {
		Token token = current_token_;
		eat(token.type);
		node = new CompareOperation(token.type, node, expr());
	}
	return node;
}

Expression *Parser::eqOrNe() {
	Expression *node = largeOrSmall();
	while (FirstIsOneOf(current_token_.type, Token::EQ, Token::NE)) {
		Token token = current_token_;
		eat(token.type);
		node = new CompareOperation(token.type, node, expr());
	}
	return node;
}

Expression *Parser::and() {
	Expression *node = eqOrNe();
	while (current_token_.type == Token::AND) {
		Token token = current_token_;
		eat(token.type);
		node = new CompareOperation(token.type, node, expr());
	}
	return node;
}

Expression *Parser::expr() {
	Expression *node = and();
	while (current_token_.type == Token::OR) {
		Token token = current_token_;
		eat(token.type);
		node = new CompareOperation(token.type, node, expr());
	}
	return node;
}



#include "Utils/UnitTest.h"
TEST_CASE(TestParserInOut) {
	const std::string source1 = R"(out "Hello, world\n"; out "Test\n"; )";
	const std::string source2 = R"(in "input", a; )";
	const std::string source3 = R"(int a; a = 10; out a; )";

	const std::string &source = source3;
	Parser parser{ source };
	parser.parse();
}
