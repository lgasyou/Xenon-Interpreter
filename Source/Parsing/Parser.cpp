#include "Parser.h"
#include "AST/Scope.h"
#include "AST/Variable.h"
#include "Utils/Exceptions.h"
#include <algorithm>

ExpressionStatement *Parser::newMainCall() {
	auto line = current_token_.line;
	auto proxy = new VariableProxy(Token(Token::IDENTIFIER, line, "main"), line);
	auto mainCall = new Call(proxy, std::vector<Expression *>(), line);
	return new ExpressionStatement(mainCall, line);
}

AstNode *Parser::parse() {
	auto b = newBlock(current_token_.line);
	if (!has_main_call_) {
		b->addStatement(newMainCall());
	}
	return b;
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
	else {
		throw EatException(current_token_.line, tokenType);
	}
}

const Token &Parser::peek() {
	if (!peeked_) {
		peeked_ = true;
		cached_token_ = scanner_.scan();
	}
	return cached_token_;
}

Statement *Parser::newStatement(bool eatSemicolon) {
	Statement *node = nullptr;

	switch (current_token_.type) {
	case Token::OUT:
		node = newOutStatement();
		if (eatSemicolon)
			eat(Token::SEMICOLON);
		break;

	case Token::IN:
		node = newInStatement();
		if (eatSemicolon)
			eat(Token::SEMICOLON);
		break;

	case Token::IDENTIFIER:
	case Token::INTEGER_LITERAL:
	case Token::REAL_LITERAL:
	case Token::STRING_LITERAL:
		if (peek().type == Token::LPAREN) {
			node = newExpressionStatement(newCall());
		} else if (peek().type == Token::SEMICOLON) {
			node = new EmptyStatement(current_token_.line);
		} else {
			node = newExpressionStatement(newAssignment());
		}
		if (eatSemicolon)
			eat(Token::SEMICOLON);
		break;

	case Token::WHILE:
		node = newWhileStatement();
		break;

	case Token::FOR:
		node = newForStatement();
		break;

	case Token::IF:
		node = newIfStatement();
		break;

	case Token::RETURN:
		node = newReturnStatememt();
		break;

	case Token::SEMICOLON:
		eat(Token::SEMICOLON);
		node = newEmptyStatement();
		break;

	case Token::DO:
		node = newDoUntilStatement();
		break;

	default:
		throw StatementException(current_token_.line);
	}

	return node;
}

Statement *Parser::newOutStatement() {
	Token token = current_token_;
	eat(Token::OUT);
	std::vector<Expression* > outMembers;
	while (current_token_.type != Token::SEMICOLON) {
		while ((current_token_.type != Token::SEMICOLON) && (current_token_.type != Token::COMMA)) {
			outMembers.push_back(parseExpression());
		}
		if (current_token_.type == Token::COMMA) {
			eat(Token::COMMA);
		}
	}
	return new OutStatement(outMembers, token.line);
}

Statement *Parser::newInStatement() {
	Token token = current_token_;
	if (peek().type != Token::STRING_LITERAL && peek().type != Token::IDENTIFIER){
		throw InException(token.line);
	}
	eat(Token::IN);
	Literal *promptString = nullptr;
	VariableProxy *variable = nullptr;
	if (current_token_.type == Token::STRING_LITERAL) {
		promptString = newLiteral();
		eat(Token::STRING_LITERAL);
		if (current_token_.type != Token::COMMA) {
			throw InException(token.line);
		}
		eat(Token::COMMA);
	}	
	if (current_token_.type != Token::IDENTIFIER) {
		throw InException(token.line);
	}
	variable = newVariableProxy();
	eat(Token::IDENTIFIER);
	return new InStatement(promptString, variable, token.line);
}

Statement *Parser::newWhileStatement() {
	Token token = current_token_;
	eat(Token::WHILE);
	Expression *whileCondition = nullptr;
	Block *whileBody = nullptr;
	whileCondition = parseExpression();
	whileBody = newBlock(token.line);
	return new WhileStatement(whileCondition, whileBody, token.line);
}

Statement *Parser::newForStatement() {
	eat(Token::FOR);
	auto &type = current_token_.type;
	if (type == Token::LPAREN)
		eat(Token::LPAREN);
	auto init = (type == Token::SEMICOLON) ? newEmptyStatement() : newStatement(/* eat semicolon */false);
	eat(Token::SEMICOLON);
	auto cond = (type == Token::SEMICOLON) ? nullptr : parseExpression();
	eat(Token::SEMICOLON);
	auto next = (type == Token::RPAREN) ? newEmptyStatement() : newStatement(/* eat semicolon */false);
	if (current_token_.type == Token::RPAREN)
		eat(Token::RPAREN);
	auto forBody = newBlock(current_token_.line);
	forBody->addStatement(next);
	return new ForStatement(init, cond, next, forBody, current_token_.line);
}

Statement *Parser::newDoUntilStatement() {
	Token token = current_token_;
	eat(Token::DO);
	Expression *untilCondition = nullptr;
	Block *doBody = nullptr;
	doBody = newBlock(token.line);
	eat(Token::UNTIL);
	untilCondition = parseExpression();
	eat(Token::SEMICOLON);
	return new DoUntilStatement(untilCondition, doBody, token.line);
}

Statement *Parser::newEmptyStatement() {
	return new EmptyStatement(current_token_.line);
}

Statement *Parser::newIfStatement() {
	Token token = current_token_;
	eat(Token::IF);
	Expression *condition = parseExpression();
	Block *thenCondition = nullptr;
	Block *elseStatement = nullptr;
	Statement *elseIfStatement = nullptr;
	if (current_token_.type == Token::LBRACE) {
		thenCondition = newBlock(current_token_.line);
	}
	if (current_token_.type == Token::ELSE) {
		eat(Token::ELSE);
		if (current_token_.type == Token::IF) {
			elseIfStatement = newIfStatement();
		} else {
			elseStatement = newBlock(current_token_.line);
		}
	}
	return new IfStatement(condition, thenCondition, elseStatement, elseIfStatement, token.line);
}

Statement *Parser::newReturnStatememt() {
	eat(Token::RETURN);
	if (current_token_.type == Token::SEMICOLON) {
		eat(Token::SEMICOLON);
		return new ReturnStatement(nullptr);
	} else {
		auto ret = new ReturnStatement(parseExpression());
		eat(Token::SEMICOLON);
		return ret;
	}
}

VariableProxy *Parser::newVariableProxy() {
	return new VariableProxy(current_token_, current_token_.line);
}

Literal *Parser::newLiteral() {
	return new Literal(current_token_, current_token_.line);
}

ExpressionStatement *Parser::newExpressionStatement(Expression *node) {
	return new ExpressionStatement(node, node->position());
}

Assignment *Parser::newAssignment() {
	VariableProxy *left = newVariableProxy();
	eat(Token::IDENTIFIER);
	Token token = current_token_;
	eat(Token::ASSIGN);
	Expression *right = parseExpression();
	return new Assignment(token.type, left, right, token.line);
}

Expression *Parser::newCall() {
	auto functionNameProxy = newVariableProxy();
	if (functionNameProxy->variable()->name() == "main") {
		has_main_call_ = true;
	}
	Token token = current_token_;
	eat(Token::IDENTIFIER);
	eat(Token::LPAREN);
	std::vector<Expression *> args;
	while (current_token_.type != Token::RPAREN 
		&& current_token_.type != Token::EOS) {
		Expression *arg = parseExpression();
		args.push_back(arg);
		if (current_token_.type == Token::COMMA) {
			eat(Token::COMMA);
		}
	}
	if (current_token_.type != Token::RPAREN) {
		throw FuncDecException(token.line);
	}
	eat(Token::RPAREN);
	return new Call(functionNameProxy, args, token.line);
}

std::vector<Declaration *> Parser::newDeclarations() {
	std::vector<Declaration *> decls;
	auto typeToken = current_token_;
	eat(current_token_.type);
	auto name = newVariableProxy();
	eat(Token::IDENTIFIER);
	Declaration *d = nullptr;
	if (current_token_.type == Token::LPAREN) {
		d = newFunctionDeclaration(name, typeToken);
		decls.push_back(d);
	} else {
		if (current_token_.type == Token::ASSIGN) {
			eat(Token::ASSIGN);
			auto rightExpr = parseExpression();
			d = newVariableDeclaration(name, rightExpr, typeToken);
		} else {
			d = newVariableDeclaration(name, nullptr, typeToken);
		}
		decls.push_back(d);
		while (current_token_.type != Token::SEMICOLON) {
			if (current_token_.type != Token::COMMA) {
				if (current_token_.type == Token::LPAREN) {
					throw FuncDecException(typeToken.line);
				}
				throw EatException(typeToken.line, Token::SEMICOLON);
			}
			eat(Token::COMMA);
			name = newVariableProxy();
			eat(Token::IDENTIFIER);
			if (current_token_.type == Token::ASSIGN) {
				eat(Token::ASSIGN);
				auto rightExpr = parseExpression();
				d = newVariableDeclaration(name, rightExpr, typeToken);
			} else {
				d = newVariableDeclaration(name, nullptr, typeToken);
			}
			decls.push_back(d);
		}
		eat(Token::SEMICOLON);
	}
	return decls;
}

static inline bool IsDeclarationStart(Token::Type type) {
	return type == Token::INT || type == Token::REAL || type == Token::STRING || type == Token::VOID;
}

std::vector<Statement *> Parser::parseBlockBody(Scope *scope) {
	std::vector<Statement *> statements;
	auto &type = current_token_.type;
	while (type != Token::RBRACE && type != Token::EOS) {
		if (IsDeclarationStart(type)) {
			const auto &decls = newDeclarations();
			statements.insert(statements.end(), decls.begin(), decls.end());
		} else {
			statements.push_back(newStatement());
		}
	}
	return statements;
}

static inline bool IsBlockStart(Token::Type t) {
	return t == Token::LBRACE;
}
static inline bool IsBlockEnd(Token::Type t) {
	return t == Token::RBRACE;
}

Block *Parser::newBlock(int line) {
	auto scope = new Scope(current_scope_);
	current_scope_ = scope;

	if (IsBlockStart(current_token_.type)) {
		eat(Token::LBRACE);
	}

	if (IsBlockStart(current_token_.type)) {
		std::vector<Statement *> block{ newBlock(current_token_.line) };
		return new Block(block, scope, current_token_.line);
	}

	const auto &statements = parseBlockBody(scope);
	if (IsBlockEnd(current_token_.type)) {
		eat(Token::RBRACE);
	}
	return new Block(statements, scope, line);
}

Declaration *Parser::newFunctionDeclaration(VariableProxy *var, const Token &tok) {
	eat(Token::LPAREN);
	std::vector<VariableDeclaration *> argumentsNodes;
	Token token = current_token_;
	VariableProxy *argument;
	Block *functionBlock = nullptr;
	while (current_token_.type != Token::RPAREN) {
		switch (current_token_.type) {
		case Token::INT:
		case Token::REAL:
		case Token::STRING: {
			token = current_token_.type;
			eat(current_token_.type);
			argument = newVariableProxy();
			eat(Token::IDENTIFIER);
			auto argumentNode = newVariableDeclaration(argument, /* initializer */nullptr, token);
			argumentsNodes.push_back(argumentNode);
			break;
		}

		case Token::COMMA:
			eat(current_token_.type);
			break;

		default:
			throw FuncDecException(tok.line);
		}
	}
	eat(Token::RPAREN);

	functionBlock = newBlock(tok.line);
	functionBlock->setIsFunctionBlock(true);

	return new FunctionDeclaration(var, tok, argumentsNodes, functionBlock, token.line);
}


VariableDeclaration *Parser::newVariableDeclaration(VariableProxy *var, Expression *initializer, const Token &tok) {
	return new VariableDeclaration(var, initializer, tok, tok.line);
}

Expression *Parser::parseBottom() {
	Expression *node = nullptr;
	Token token = current_token_;
	switch (token.type) {
	case Token::ADD:
	case Token::SUB:
	case Token::NOT:
		eat(token.type);
    return new UnaryOperation(token.type, parseBottom(), token.line);

	case Token::INTEGER_LITERAL:
	case Token::REAL_LITERAL:
	case Token::STRING_LITERAL:
		eat(token.type);
		node = new Literal(token, token.line);
		break;

	case Token::IDENTIFIER:
		if (peek().type == Token::LPAREN) {
			node = newCall();
		}
		else {
			eat(Token::IDENTIFIER);
			node = new VariableProxy(token, token.line);
		}
		break;
	
	case Token::LPAREN:
		eat(Token::LPAREN);
		node = parseExpression();
		eat(Token::RPAREN);
		break; 

	default:
		throw OpException(token.line);
	}
	token = current_token_;
	switch (token.type) {
	case Token::INV:
		eat(token.type);
		return new BinaryOperation(token.type, node, parseBottom(), token.line);
	}
	return node;
}

Expression *Parser::parseInvolution() {
	Expression *node = parseBottom();
	while (FirstIsOneOf(current_token_.type, Token::INV)) {
		Token token = current_token_;
		eat(token.type);
		node = new BinaryOperation(token.type, node, parseBottom(), token.line);
	}
	return node;
}

Expression *Parser::parseMulOrDivExpression() {
	Expression *node = parseInvolution();
	while (FirstIsOneOf(current_token_.type, Token::MUL, Token::DIV, Token::MOD)) {
		Token token = current_token_;
		eat(token.type);
		node = new BinaryOperation(token.type, node, parseInvolution(), token.line);
	}
	return node;
}

Expression *Parser::parseAddOrSubExpression() {
	Expression *node = parseMulOrDivExpression();
	while (FirstIsOneOf(current_token_.type, Token::ADD, Token::SUB, Token::STRING_CONCAT, Token::STRING_DELETE)) {
		Token token = current_token_;
		eat(token.type);
		node = new BinaryOperation(token.type, node, parseMulOrDivExpression(), token.line);
	}
	return node;
}

Expression *Parser::parseLessOrGreaterExpression() {
	Expression *node = parseAddOrSubExpression();
	while (FirstIsOneOf(current_token_.type, Token::LT, Token::GT, Token::LTE, Token::GTE)) {
		Token token = current_token_;
		eat(token.type);
		node = new CompareOperation(token.type, node, parseAddOrSubExpression(), token.line);
	}
	return node;
}

Expression *Parser::parseEqOrNeExpression() {
	Expression *node = parseLessOrGreaterExpression();
	while (FirstIsOneOf(current_token_.type, Token::EQ, Token::NE)) {
		Token token = current_token_;
		eat(token.type);
		node = new CompareOperation(token.type, node, parseLessOrGreaterExpression(), token.line);
	}
	return node;
}

Expression *Parser::parseAndExpression() {
	Expression *node = parseEqOrNeExpression();
	while (current_token_.type == Token::AND) {
		Token token = current_token_;
		eat(token.type);
		node = new CompareOperation(token.type, node, parseEqOrNeExpression(), token.line);
	}
	return node;
}

Expression *Parser::parseOrExpression() {
	Expression *node = parseAndExpression();
	while (current_token_.type == Token::OR) {
		Token token = current_token_;
		eat(token.type);
		node = new CompareOperation(token.type, node, parseAndExpression(), token.line);
	}
	return node;
}

Expression *Parser::parseExpression() {
	Expression *node = parseOrExpression();
	while (current_token_.type == Token::ASSIGN) {
		Token token = current_token_;
		eat(token.type);
		auto lhs = static_cast<VariableProxy *>(node);
		node = new Assignment(token.type, lhs, parseOrExpression(), token.line);
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
