#pragma once

#include "Stable.h"
#include <string>
#include <vector>

#include "Token.h"

class Scanner {
public:
	Scanner(const std::string &text) 
		: text_(text) {
		current_char_ = text[pos_];
	}

	Token scan();

private:
	void skipSingleLineComment();

	void skipWhitespace();

	Token scanIdentifierOrKeyword();

	Token scanNumber();

	Token scanString();

	void scanEscape();

	bool isIdentifierBegin(char ch);

	void advance();

	char peek();

	// TODO
	void error();

private:
	Token::Type current_token_;
	int pos_ = 0;
	char current_char_;
	const std::string &text_;
	// Stores the raw string of number, identifier or string.
	std::string string_value_;
};
