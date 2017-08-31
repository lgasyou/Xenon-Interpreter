#pragma once

#include "Stable.h"
#include <string>
#include <vector>

#include "Token.h"

class Scanner {
public:
	Scanner(const std::string &text) {
		current_char_ = text[pos_];
		text_ = text;
	}

	Token::Value scan();

	const std::string &numberValue() const {
		return number_value_;
	}

private:
	// TODO
	Token::Value skipSingleLineComment();

	Token::Value skipWhitespace();

	Token::Value scanIdentifierOrKeyword();

	Token::Value scanNumber();

	// TODO
	Token::Value scanString();

	void advance();

	char peek();

	// TODO
	void error();

private:
	Token::Value current_token_;
	int pos_ = 0;
	char current_char_;
	std::string text_;
	std::string number_value_; //store var's value;
	std::string string_value_; //store Identifier's name and maybe type String's value
};
