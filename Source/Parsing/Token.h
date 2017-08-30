#pragma once

#include "Stable.h"

//   T: Non-keyword tokens
//   K: Keyword tokens
#define TOKEN_LIST(T, K)												\
	/* End of source indicator. */										\
	T(EOS, "EOS", 0)													\
																		\
	/* Punctuators (ECMA-262, section 7.7, page 15). */					\
	T(LPAREN, "(", 0)													\
	T(RPAREN, ")", 0)													\
	T(LBRACK, "[", 0)													\
	T(RBRACK, "]", 0)													\
	T(LBRACE, "{", 0)													\
	T(RBRACE, "}", 0)													\
	T(SEMICOLON, ";", 0)												\
	T(PERIOD, ".", 0)													\
																		\
	/* Assignment operators. */											\
	/* IsAssignmentOp() relies on this block of enum values being */	\
	/* contiguous and sorted in the same order! */						\
	T(INIT, "=init", 2) /* AST-use only. */								\
	T(ASSIGN, "=", 2)													\
																		\
	/* Binary operators sorted by precedence. */						\
	/* IsBinaryOp() relies on this block of enum values */				\
	/* being contiguous and sorted in the same order! */				\
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
	/* IsCompareOp() relies on this block of enum values */				\
	/* being contiguous and sorted in the same order! */				\
	T(EQ, "==", 9)														\
	T(NE, "<>", 9)														\
	T(LT, "<", 10)														\
	T(GT, ">", 10)														\
	T(LTE, "<=", 10)													\
	T(GTE, ">=", 10)													\
																		\
	/* Unary operators. */												\
	/* IsUnaryOp() relies on this block of enum values */				\
	/* being contiguous and sorted in the same order! */				\
	T(NOT, "!", 0)														\
	T(BIT_NOT, "~", 0)													\
	K(VOID, "void", 0)													\
																		\
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
	/* Literals (ECMA-262, section 7.8, page 16). */					\
	T(NUMBER_LITERAL, NULL, 0)											\
	T(SMI, NULL, 0)														\
	T(STRING_LITERAL, NULL, 0)											\
																		\
	/* Identifiers (not keywords or future reserved words). */			\
	T(IDENTIFIER, NULL, 0)												\
																		\
	/* Illegal token - not able to scan. */								\
	T(ILLEGAL, "ILLEGAL", 0)											\
	T(ESCAPED_KEYWORD, NULL, 0)											\
	T(ESCAPED_STRICT_RESERVED_WORD, NULL, 0)							\
																		\
	/* Scanner-internal use only. */									\
	T(WHITESPACE, NULL, 0)												\
	T(UNINITIALIZED, NULL, 0)											\
	T(REGEXP_LITERAL, NULL, 0)		

class Token {
public:
#define T(name, string, precedence) name,
	enum Value {
		TOKEN_LIST(T, T)
		NUM_TOKENS
	};
#undef T

	// Predicates
	static bool IsKeyword(Value tok) {
		return token_type[tok] == 'K';
	}

	static bool IsIdentifier(Value tok) {
		return tok == IDENTIFIER;
	}

	static bool IsAssignmentOp(Value tok) {
		return INIT <= tok && tok <= ASSIGN;
	}

	static bool IsBinaryOp(Value op) { return COMMA <= op && op <= STRING_DELETE; }

	static bool IsCompareOp(Value op) {
		return EQ <= op && op <= GTE;
	}

	static bool IsOrderedRelationalCompareOp(Value op) {
		return op == LT || op == LTE || op == GT || op == GTE;
	}

	static bool IsEqualityOp(Value op) {
		return op == EQ;
	}

	static bool IsInequalityOp(Value op) {
		return op == NE;
	}

	static bool IsArithmeticCompareOp(Value op) {
		return IsOrderedRelationalCompareOp(op) ||
			IsEqualityOp(op) || IsInequalityOp(op);
	}

	static Value NegateCompareOp(Value op) {
		switch (op) {
		case EQ: return NE;
		case NE: return EQ;
		case LT: return GTE;
		case GT: return LTE;
		case LTE: return GT;
		case GTE: return LT;
		}
	}

	static Value ReverseCompareOp(Value op) {
		switch (op) {
		case EQ: return EQ;
		case NE: return NE;
		case LT: return GT;
		case GT: return LT;
		case LTE: return GTE;
		case GTE: return LTE;
		}
	}

	static bool EvalComparison(Value op, double op1, double op2) {
		switch (op) {
		case Token::EQ:
		case Token::NE: return (op1 != op2);
		case Token::LT: return (op1 < op2);
		case Token::GT: return (op1 > op2);
		case Token::LTE: return (op1 <= op2);
		case Token::GTE: return (op1 >= op2);
		}
	}

	static bool IsUnaryOp(Value op) {
		return (NOT <= op && op <= VOID) || op == ADD || op == SUB;
	}

private:
	static const char* const name_[NUM_TOKENS];
	static const char* const string_[NUM_TOKENS];
	static const uint8_t string_length_[NUM_TOKENS];
	static const int8_t precedence_[NUM_TOKENS];
	static const char token_type[NUM_TOKENS];
};
