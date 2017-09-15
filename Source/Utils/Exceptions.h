#pragma once

#include "Stable.h"
#include "Parsing/Token.h"
#include <exception>

class Exception : public std::exception {
public:
	Exception(int line)
		: std::exception(), exception_line_(line) {}
private:
	int exception_line_;
};

class EatException : public Exception {
public:
	EatException(int line, Token::Type type)
		: Exception(line), need_type_(type) {
		if (type != Token::STRING_LITERAL
			&& type != Token::INTEGER_LITERAL
			&& type != Token::REAL_LITERAL
			&& type != Token::IDENTIFIER
			&& type != Token::COMMA) {
			rea_ = token.String(type);

			std::cout << "解析出现错误\n错误信息：需要一个\"" << rea_ << "\"\n错误位置：Line:" << line << "\n";
		}
		else if (type == Token::IDENTIFIER) {
			std::cout << "解析出现错误\n错误信息：需要一个变量\n错误位置：Line:" << line << "\n";
		}
		else if (type == Token::COMMA) {
			std::cout << "解析出现错误\n错误信息：需要一个\",\"或\";\"\n错误位置：Line:" << line << "\n";
		}
		else {
			std::cout << "解析出现错误\n错误信息：需要一个数据\n错误位置：Line:" << line << "\n";
		}
	}

private:
	Token token;
	Token::Type	need_type_;
	const char* rea_;
};

class StatementException : public Exception {
public:
	StatementException(int line)
		: Exception(line){
		std::cout << "解析出现错误\n错误信息：语法错误\n错误位置：Line:" << line << "\n";
	}
};

class OutException : public Exception {
public:
	OutException(int line)
		: Exception(line) {
		std::cout << "解析出现错误\n错误信息：输出格式错误\n错误位置：Line:" << line << "\n";
	}
};

class InException : public Exception {
public:
	InException(int line)
		: Exception(line) {
		std::cout << "解析出现错误\n错误信息：输入格式错误\n错误位置：Line:" << line << "\n";
	}
};

class OpException : public Exception {
public:
	OpException(int line)
		: Exception(line) {
		std::cout << "解析出现错误\n错误信息：表达式错误\n错误位置：Line:" << line << "\n";
	}
};

class FuncDecException : public Exception {
public:
	FuncDecException(int line)
		: Exception(line) {
		std::cout << "解析出现错误\n错误信息：函数声明错误\n错误位置：Line:" << line << "\n";
	}
	FuncDecException(std::string s)
		: Exception(0) {
		std::cout << "解析出现错误\n错误信息：需要一个主函数\n";
	}
};

class ScanException : public Exception {
public:
	ScanException(int line)
		: Exception(line) {
		std::cout << "解析出现错误\n错误信息：非法字符\n错误位置：Line:" << line << "\n";
	}
};

class BracketsException : public Exception {
public:
	BracketsException(int line)
		: Exception(line) {
		std::cout << "解析出现错误\n错误信息：括号匹配错误\n错误位置：Line:" << line << "\n";
	}
};

class ValueException : public Exception {
public:
	ValueException(int line)
		: Exception(line) {
		std::cout << "解析出现错误\n错误信息：数据类型错误\n";
	}
};