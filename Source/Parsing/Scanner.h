#pragma once

#include <string>
#include <vector>

#include "Token.h"

class Scanner {
public:
	Token::Value scan(std::string code);

	Token::Value skipSingleLineComment();

	Token::Value skipWhitespace();

	Token::Value scanIdentifierOrKeyword();

	Token::Value scanNumber();

	Token::Value scanString();

	void advance();

	Token::Value currentToken() const {
		return curTok;
	}

private:
	Token::Value curTok;
};
