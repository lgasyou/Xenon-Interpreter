#pragma once

#include "Stable.h"
#include <string>
#include <iostream>
#include "Utils/Zone.h"

class AstValue : public ZoneObject {
public:
	enum Type {
		INTEGER,
		REAL,
		STRING,
	};

	explicit AstValue(int integer);
	explicit AstValue(float real);
	explicit AstValue(const std::string &string);

	~AstValue();

	int toInt() const;
	float toReal() const;
	const std::string &toString() const;

	Type type() const { return type_; }

private:
	union {
		int integer;
		float real;
		std::string *string;
	} var;

	Type type_;
};

inline std::ostream &operator<<(std::ostream &os, const AstValue &var) {
	switch (var.type()) {
	case AstValue::INTEGER:
		os << var.toInt();

	case AstValue::REAL:
		os << var.toReal();

	case AstValue::STRING:
		os << var.toString();
	}
	return os;
}

inline std::ostream &operator<<(std::ostream &os, AstValue &var) {
	return operator<<(os, static_cast<const AstValue &>(var));
}
