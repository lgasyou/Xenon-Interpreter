#pragma once

#include "Stable.h"

//   T: Non-keyword tokens
//   K: Keyword tokens
#define TOKEN_LIST(T, K)												\
	/* End of source indicator. */										\
	T(EOS, "EOS", 0)													\
																		\
	/* Punctuators. */													\
	T(LPAREN, "(", 0)													\
	T(RPAREN, ")", 0)													\
	T(LBRACE, "{", 0)													\
	T(RBRACE, "}", 0)													\
	T(SEMICOLON, ";", 0)												\
																		\
	/* Assignment operators. */											\
	T(ASSIGN, "=", 2)													\
																		\
	/* Binary operators sorted by precedence. */						\
	T(COMMA, ",", 1)													\
	T(OR, "||", 4)														\
	T(AND, "&&", 5)														\
	T(ADD, "+", 12)														\
	T(SUB, "-", 12)														\
	T(MUL, "*", 13)														\
	T(DIV, "/", 13)														\
	T(MOD, "%", 13)														\
	/* String Operators */												\
	T(STRING_CONCAT, "$", 13)										    \
	T(STRING_DELETE , "#", 13)											\
																		\
	/* Compare operators sorted by precedence. */						\
	T(EQ, "==", 9)														\
	T(NE, "<>", 9)														\
	T(LT, "<", 10)														\
	T(GT, ">", 10)														\
	T(LTE, "<=", 10)													\
	T(GTE, ">=", 10)													\
																		\
	/* Unary operators. */												\
	T(NOT, "!", 0)														\
																		\
	K(VOID, "void", 0)													\
	K(ELSE, "else", 0)													\
	K(IF, "if", 0)														\
	K(RETURN, "return", 0)												\
	K(INT, "int", 0)													\
	K(REAL, "real", 0)													\
	K(STRING, "string", 0)												\
	K(OUT, "out", 0)													\
	K(IN, "in", 0)														\
	K(WHILE, "while", 0)												\
																		\
	/* Literals. */														\
	T(INTEGER_LITERAL, NULL, 0)											\
	T(REAL_LITERAL, NULL, 0)											\
	T(STRING_LITERAL, NULL, 0)											\
																		\
	/* Identifiers (not keywords). */									\
	T(IDENTIFIER, NULL, 0)												\
																		\
	/* Illegal token - not able to scan. */								\
	T(ILLEGAL, "ILLEGAL", 0)											

class Token {
public:
	// Stores all token values.
#define T(name, string, precedence) name,
	enum Type {
		TOKEN_LIST(T, T)
		NUM_TOKENS
	};
#undef T

public:
	Token(Type type = EOS, const std::string &value = std::string());

	Token &operator=(const Token &rhs) {
		type = rhs.type;
		value = rhs.value;
		return *this;
	}

	Type type;
	std::string value;

public:
	// Returns a string corresponding to the C++ token name
	// (e.g. "LT" for the token LT).
	static const char* Name(Type tok) {
		return names_[tok];
	}

	// Returns a string corresponding to the min-C token string
	// (.e., "<" for the token LT) or NULL if the token doesn't
	// have a (unique) string (e.g. an IDENTIFIER).
	static const char* String(Type tok) {
		return strings_[tok];
	}

	// Returns a enum value corresponding to the min-C token string
	// (.e., Token::INT for "int"
	static Token::Type GetType(const std::string &string);

	static bool Includes(const std::string &string);
	static bool Includes(Token::Type value);

	// Returns the precedence > 0 for binary and compare
	// operators; returns 0 otherwise.
	static int Precedence(Type tok) {
		return precedences_[tok];
	}

private:
	static const char* const names_[NUM_TOKENS];
	static const char* const strings_[NUM_TOKENS];
	static const int8_t precedences_[NUM_TOKENS];
};
