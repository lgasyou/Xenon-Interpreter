#pragma once

#include "Stable.h"
#include <string>
#include <iostream>

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

	int toInt() const;

	float toReal() const;

	std::string toString() const;

	Type type() const { return type_; }

private:
	union {
		int integer;
		float real;
		std::string *string;
	} var;

	Type type_;
};

inline std::ostream &operator<<(std::ostream &os, const Variable &var) {
	switch (var.type()) {
	case Variable::INTEGER:
		os << var.toInt();

	case Variable::REAL:
		os << var.toReal();

	case Variable::STRING:
		DBG_PRINT << var.toString();
		os << var.toString();
	}
	return os;
}

inline std::ostream &operator<<(std::ostream &os, Variable &var) {
	const Variable &v = var;
	return operator<<(os, var);
}
