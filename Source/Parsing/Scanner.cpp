#include "Scanner.h"
#include <cctype>

// Returns true if pos is out of str's range.
static inline bool outOfRange(int pos, const std::string &str) {
	return pos > str.length() - 1;
}

Token Scanner::scan() {
	while (current_char_ != '\0') {
		switch (current_char_) {
		case ' ':
		case '\t':
		case '\n':
			skipWhitespace();
			break;

		case '+':
			advance();
			return Token(Token::ADD);

		case '-':
			advance();
			return Token(Token::SUB);

		case '*':
			advance();
			return Token(Token::MUL);

		case '%':
			advance();
			return Token(Token::MOD);

		case '/':
			// / //
			if (peek() == '/') {
				skipSingleLineComment();
			} else {
				advance();
				return Token(Token::DIV);
			}
			break;

		case ',':
			advance();
			return Token(Token::COMMA);

		case ';':
			advance();
			return Token(Token::SEMICOLON);

		case '(':
			advance();
			return Token(Token::LPAREN);

		case ')':
			advance();
			return Token(Token::RPAREN);

		case '{':
			advance();
			return Token(Token::LBRACE);

		case '}':
			advance();
			return Token(Token::RBRACE);

		case '=':
			// = ==
			if (peek() == '=') {
				advance();
				advance();
				return Token(Token::EQ);
			}
			advance();
			return Token(Token::ASSIGN);

		case '>':
			// > >=
			if (peek() == '=') {
				advance();
				advance();
				return Token(Token::GTE);
			}
			advance();
			return Token(Token::GT);

		case '<':
			// < <= <>
			if (peek() == '=') {
				advance();
				advance();
				return Token(Token::LTE);
			}
			if (peek() == '>') {
				advance();
				advance();
				return Token(Token::NE);
			}
			advance();
			return Token(Token::LT);

		case '&':
			// & &&
			if (peek() == '&') {
				advance();
				advance();
				return Token(Token::AND);
			}
			error();
			break;

		case '|':
			// | ||
			if (peek() == '|') {
				advance();
				advance();
				return Token(Token::OR);
			}
			error();
			break;

		case '!':
			advance();
			return Token(Token::NOT);

		case '"':
			return scanString();

		case '#':
			advance();
			return Token(Token::STRING_DELETE);

		case '$':
			advance();
			return Token(Token::STRING_CONCAT);

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

void Scanner::skipSingleLineComment() {
	while (current_char_ != '\0' && current_char_ != '\n') {
		advance();
	}
	advance();
}

void Scanner::skipWhitespace() {
	advance();
}

Token Scanner::scanNumber() {
	string_value_.clear();
	while (current_char_ != 0 && isdigit(current_char_)) {
		string_value_ += current_char_;
		advance();
	}
	if (current_char_ == '.') {
		string_value_ += current_char_;
		advance();
		while (current_char_ != 0 && isdigit(current_char_)) {
			string_value_ += current_char_;
			advance();
		}
		return Token(Token::REAL_LITERAL, string_value_);
	}
	return Token(Token::INTEGER_LITERAL, string_value_);
}

Token Scanner::scanString() {
	string_value_.clear();
	advance();
	while (current_char_ != '"') {
		string_value_ += current_char_;
		advance();
	}
	advance();
	return Token(Token::STRING_LITERAL, string_value_);
}

Token Scanner::scanIdentifierOrKeyword() {
	string_value_.clear();
	bool hasUnderline = false;
	while (current_char_ != 0 && (isalnum(current_char_) || current_char_ == '_')) {
		if (current_char_ == '_') {
			hasUnderline = 1;
		}
		string_value_ += current_char_;
		advance();
	}
	return Token(Token::GetValue(string_value_), string_value_);
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


#include "Utils/FileReader.h"
#include "Utils/UnitTest.h"
TEST_CASE(ScanFromFile) {
	FileReader reader{ "TestSamples/scanner_test.txt" };
	const std::string source = reader.readAll();
	DBG_PRINT << "Input:\n";
	DBG_PRINT << source << "\n";
	DBG_PRINT << "Output:\n";
	Scanner scanner{ source };
	Token value{ Token::EOS };
	while ((value = scanner.scan()).type != Token::EOS) {
		auto string = Token::Name(value.type);
		DBG_PRINT << string << "\n";
	}
}

TEST_CASE(ScanFromStringLiteral) {
	const std::string source1 = R"(+-*/,;()int{}= == > >= < <= <> && || ! # $ 123 int hello)";
	const std::string source2 = R"(int main() { return 0; })";
	const std::string source3 = R"("Hello";out in int hello;)";
	const std::string source4 = R"(int main() { out "Hello, world!\n"; return 0; })";

	const std::string &source = source4;
	DBG_PRINT << "Input:\n";
	DBG_PRINT << source << "\n";
	DBG_PRINT << "Output:\n";
	Scanner scanner{ source };
	Token value{ Token::EOS };
	while ((value = scanner.scan()).type != Token::EOS) {
		auto string = Token::Name(value.type);
		DBG_PRINT << string << "\n";
	}
}
