#pragma once

#include "Stable.h"
#include <string>
#include <vector>

#include "Token.h"

class Scanner {
public:
	Scanner(const std::string &source);

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
	const std::string &text_;
};
