#include "Token.h"
#include <algorithm>
#include <string>

#define T(name, string, precedence) #name,
const char* const Token::names_[NUM_TOKENS] = { TOKEN_LIST(T, T) };
#undef T


#define T(name, string, precedence) string,
const char* const Token::strings_[NUM_TOKENS] = { TOKEN_LIST(T, T) };
#undef T

constexpr uint8_t length(const char* str) {
	return str ? static_cast<uint8_t>(strlen(str)) : 0;
}
#define T(name, string, precedence) length(string),
const uint8_t Token::string_lengths_[NUM_TOKENS] = { TOKEN_LIST(T, T) };
#undef T

#define T(name, string, precedence) precedence,
const int8_t Token::precedences_[NUM_TOKENS] = { TOKEN_LIST(T, T) };
#undef T


#define KT(a, b, c) 'T',
#define KK(a, b, c) 'K',
const char Token::token_types_[] = { TOKEN_LIST(KT, KK) };
#undef KT
#undef KK

Token::Value Token::GetValue(const std::string &string) {
	int current = -1;
	std::find_if(std::cbegin(strings_), std::cend(strings_), [&](const char *s) {
		current += 1;
		if (!s) return false;
		return strcmp(string.c_str(), s) == 0;
	});
	Token::Value value = static_cast<Token::Value>(current);
	return Includes(value) ? Token::Value(value) : Token::IDENTIFIER;
}

bool Token::Includes(Token::Value value) {
	return value != NUM_TOKENS - 1;
}


#include "Utils/UnitTest.h"
TEST_CASE(Token) {
	ASSERT_EQ(Token::IDENTIFIER, Token::GetValue("i"));
	ASSERT_EQ(Token::INT, Token::GetValue("int"));
	ASSERT_EQ(Token::REAL, Token::GetValue("real"));
}
