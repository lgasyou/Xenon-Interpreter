#pragma once

#include <string>
#include <vector>

#include "Token.h"

class Scanner;

class Lexer {
public:
	Lexer();
	~Lexer() { delete scanner_; }

private:
	Scanner *scanner_;
};


class Scanner {
public:
	Token::Value scan();

	void advance();

	Token::Value currentToken() const {
		return current_token_;
	}

private:
	Token::Value skipSingleLineComment();

	Token::Value skipWhitespace();

	Token::Value scanIdentifierOrKeyword();

	Token::Value scanNumber();

	Token::Value scanString();

private:
	Token::Value current_token_;
};
