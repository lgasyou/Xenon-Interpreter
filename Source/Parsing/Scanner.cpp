#include "Scanner.h"
#include <cctype>

// Returns true if pos is out of range.
static inline bool outOfRange(int pos, const std::string &str) {
	return pos > str.length() - 1;
}

Token::Value Scanner::scan() {
	while (current_char_ != '\0') {
		switch (current_char_) {
		case ' ':
		case '\t':
		case '\n':
			skipWhitespace();
			break;

		case '+':
			advance();
			return Token::ADD;

		case '-':
			advance();
			return Token::SUB;

		case '*':
			advance();
			return Token::MUL;

		case '%':
			advance();
			return Token::MOD;

		case '/':
			// / //
			if (peek() == '/') {
				while (current_char_ != '\n') {
					advance();
				}
				advance();
			} else {
				advance();
				return Token::DIV;
			}
			break;

		case ',':
			advance();
			return Token::COMMA;

		case ';':
			advance();
			return Token::SEMICOLON;

		case '(':
			advance();
			return Token::LPAREN;

		case ')':
			advance();
			return Token::RPAREN;

		case '{':
			advance();
			return Token::LBRACE;

		case '}':
			advance();
			return Token::RBRACE;

		case '=':
			// = ==
			if (peek() == '=') {
				advance();
				advance();
				return Token::EQ;
			}
			advance();
			return Token::ASSIGN;

		case '>':
			// > >=
			if (peek() == '=') {
				advance();
				advance();
				return Token::GTE;
			}
			advance();
			return Token::GT;

		case '<':
			// < <= <>
			if (peek() == '=') {
				advance();
				advance();
				return Token::LTE;
			}
			if (peek() == '>') {
				advance();
				advance();
				return Token::NE;
			}
			advance();
			return Token::LT;

		case '&':
			// & &&
			if (peek() == '&') {
				advance();
				advance();
				return Token::AND;
			}
			error();
			break;

		case '|':
			// | ||
			if (peek() == '|') {
				advance();
				advance();
				return Token::OR;
			}
			error();
			break;

		case '!':
			advance();
			return Token::NOT;

		case '"':
			return scanString();
			//advance();
			//return Token::DQUOTAYION;

		case '#':
			advance();
			return Token::STRING_DELETE;

		case '$':
			advance();
			return Token::STRING_CONCAT;

		default:
			if (isalpha(current_char_)) {
				return scanIdentifierOrKeyword();
			}
			if (isdigit(current_char_)) {
				return scanNumber();
			}
			error();
			break;
		}
	}
	return Token::EOS;
}

Token::Value Scanner::skipSingleLineComment() {
	return Token::Value();
}

Token::Value Scanner::skipWhitespace() {
	advance();
	return Token::WHITESPACE;
}

void Scanner::advance() {
	pos_ += 1;
	current_char_ = outOfRange(pos_, text_) ? 0 : text_[pos_];
}

char Scanner::peek() {
	int peekPos = pos_ + 1;
	return outOfRange(peekPos, text_) ? 0 : text_[peekPos];
}

void Scanner::error() {

}

Token::Value Scanner::scanNumber() {
	std::string result;
	while (current_char_ != 0 && isdigit(current_char_)) {
		result += current_char_;
		advance();
	}
	if (current_char_ == '.') {
		result += current_char_;
		advance();
		while (current_char_ != 0 && isdigit(current_char_)) {
			result += current_char_;
			advance();
		}
		number_value_ = result;
		return Token::REAL_LITERAL;
	} else {
		number_value_ = result;
		return Token::INTEGER_LITERAL;
	}

}

Token::Value Scanner::scanString() {
	return Token::STRING_LITERAL;
}

Token::Value Scanner::scanIdentifierOrKeyword() {
	std::string result;
	int flag = 0;
	while (current_char_ != 0 && (isalnum(current_char_) || current_char_ == '_')) {
		if (current_char_ == '_') {
			flag = 1;
		}
		result += current_char_;
		advance();
	}
	string_value_ = result;
	return flag == 1 ? Token::IDENTIFIER : Token::GetValue(result);
}


#include "Utils/FileReader.h"
#include "Utils/UnitTest.h"
TEST_CASE(Scanner) {
	FileReader reader{ "TestSamples/scanner_test.txt" };
	Scanner scanner{ reader.readAll() };
	int value = 0;
	while ((value = scanner.scan()) != Token::EOS) {
		auto string = Token::Name((Token::Value)value);
		DBG_PRINT << string << "\n";
	}
}
