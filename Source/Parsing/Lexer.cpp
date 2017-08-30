#include "Lexer.h"

Lexer::Lexer()
	: scanner_(new Scanner) {}


Token::Value Scanner::scan() {
	return Token::Value();
}

void Scanner::advance() {

}
