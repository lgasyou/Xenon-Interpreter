#pragma once

#include "Stable.h"
#include <string>

class Variable {
public:
	enum Type {
		INTEGER,
		REAL,
		STRING,
	};

	Variable(int integer);
	Variable(float real);
	Variable(const std::string &string);

	int toInt();

	float toReal();

	std::string toString();

	Type type() const { return type_; }

private:
	union {
		int integer;
		float real;
		std::string string;
	};

	Type type_;
};