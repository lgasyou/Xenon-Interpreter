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

	explicit Variable(int integer);
	explicit Variable(float real);
	explicit Variable(const std::string &string);

	~Variable();

	int toInt();

	float toReal();

	std::string toString();

	Type type() const { return type_; }

private:
	union {
		int integer;
		float real;
		std::string *string;
	} var;

	Type type_;
};