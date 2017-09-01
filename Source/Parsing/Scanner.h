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
	// TODO
	void skipSingleLineComment();

	void skipWhitespace();

	Token scanIdentifierOrKeyword();

	Token scanNumber();

	// TODO
	Token scanString();

	void advance();

	char peek();

	// TODO
	void error();

private:
	Token::Type current_token_;
	int pos_ = 0;
	char current_char_;
	const std::string &text_;
	std::string string_value_; //store Identifier's name and maybe type String's value
};
