#pragma once

#include "Stable.h"
#include <string>
#include <vector>

#include "Token.h"

class Scanner {
public:
	Scanner(const std::string &text) 
		: text_(text) {
		current_char_ = text[cursor_];
	}

	// Gets next token.
	Token scan();

private:
	void skipSingleLineComment();

	void skipWhitespace();

	Token scanIdentifierOrKeyword();

	Token scanNumber();

	Token scanString();

	void scanEscape();

	// Low-level scanning support.
	void advance();

	// Gets next character without move cursor.
	char peek();

private:
	Token::Type current_token_;
	int cursor_ = 0;
	char current_char_;
	const std::string &text_;
	// Stores the raw string of number, identifier or string.
	std::string string_value_;
};
