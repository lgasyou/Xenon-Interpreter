#include "Scanner.h"
#include <cctype>

// Returns true if pos is out of str's range.
static inline bool outOfRange(int pos, const std::string &str) {
	return pos > str.length() - 1;
}

static inline bool isIdentifierBegin(char ch) {
	return isalpha(ch) || ch == '_';
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
			UNREACHABLE();

		case '|':
			// | ||
			if (peek() == '|') {
				advance();
				advance();
				return Token(Token::OR);
			}
			UNREACHABLE();

		case '!':
			// ! !=
			if (peek() == '=') {
				advance();
				advance();
				return Token(Token::NE);
			}
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
			if (isIdentifierBegin(current_char_)) {
				return scanIdentifierOrKeyword();
			}
			if (isdigit(current_char_)) {
				return scanNumber();
			}
			UNREACHABLE();
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
	advance(); // Eat first '"'
	while (current_char_ != '"' && current_char_ != '\0') {
		if (current_char_ == '\\') {
			scanEscape();
		} else {
			string_value_ += current_char_;
		}
		advance();
	}
	advance(); // Eat second '"'
	return Token(Token::STRING_LITERAL, string_value_);
}

void Scanner::scanEscape() {
	switch (peek()) {
	case 'n':
		string_value_ += '\n';
		advance();
		break;

	case'\\':
		string_value_ += '\\';
		advance();
		break;

	case'"':
		string_value_ += '"';
		advance();
		break;
	}
}

Token Scanner::scanIdentifierOrKeyword() {
	string_value_.clear();
	while (current_char_ != 0 && (isalnum(current_char_) || current_char_ == '_')) {
		string_value_ += current_char_;
		advance();
	}
	return Token(Token::GetType(string_value_), string_value_);
}

void Scanner::advance() {
	cursor_ += 1;
	current_char_ = outOfRange(cursor_, text_) ? 0 : text_[cursor_];
}

char Scanner::peek() {
	int peekPos = cursor_ + 1;
	return outOfRange(peekPos, text_) ? 0 : text_[peekPos];
}


#include "Utils/FileReader.h"
#include "Utils/UnitTest.h"
TEST_CASE(ScanFromFile) {
	FileReader reader{ "TestCases/scanner.test" };
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
	const std::string source5 = R"("\"\n\\")";

	const std::string &source = source5;
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
