#include "Scanner.h"

Scanner::Scanner(const std::string &source)
	: text_(source) {}

Token::Value Scanner::scan() {
	DBG_PRINT << text_;
	return Token::Value();
}

void Scanner::advance() {

}
