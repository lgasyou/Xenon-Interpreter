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
	AstValue(const AstValue &rhs);

	~AstValue();

	int toInt() const;
	float toReal() const;
	const std::string &toString() const;

	Type type() const { return type_; }

	AstValue operator+(const AstValue &rhs);
	AstValue operator-(const AstValue &rhs);
	AstValue operator*(const AstValue &rhs);
	AstValue operator/(const AstValue &rhs);
	AstValue operator%(const AstValue &rhs);
	bool operator!();

	bool operator==(const AstValue &rhs);
	bool operator!=(const AstValue &rhs);
	bool operator<(const AstValue &rhs);
	bool operator<=(const AstValue &rhs);
	bool operator>(const AstValue &rhs);
	bool operator>=(const AstValue &rhs);

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
		break;

	case AstValue::REAL:
		os << var.toReal();
		break;

	case AstValue::STRING:
		os << var.toString();
		break;
	}
	return os;
}

inline std::ostream &operator<<(std::ostream &os, AstValue &var) {
	return operator<<(os, static_cast<const AstValue &>(var));
}

inline AstValue AstValue::operator+(const AstValue &rhs) {
	if (type() == REAL || rhs.type() == REAL) {
		return AstValue(toReal() + rhs.toReal());
	} else if (type() == INTEGER || rhs.type() == INTEGER) {
		return AstValue(toInt() + rhs.toInt());
	}
	return AstValue(toString() + rhs.toString());
}

inline AstValue AstValue::operator*(const AstValue &rhs) {
	if (type() == REAL || rhs.type() == REAL) {
		return AstValue(toReal() + rhs.toReal());
	}
	return AstValue(toInt() + rhs.toInt());
}

inline AstValue AstValue::operator/(const AstValue &rhs) {
	if (type() == REAL || rhs.type() == REAL) {
		return AstValue(toReal() + rhs.toReal());
	}
	return AstValue(toInt() + rhs.toInt());
}

inline AstValue AstValue::operator%(const AstValue & rhs) {
	return AstValue(toInt() % rhs.toInt());
}

inline bool AstValue::operator!() {
	if (type() == INTEGER) {
		return !toInt();
	}
	return false;
}

inline bool AstValue::operator==(const AstValue & rhs) {
	if (type() != STRING || rhs.type() != STRING) {
		return toReal() == rhs.toReal();
	}
	return toString() == rhs.toString();
}

inline bool AstValue::operator!=(const AstValue &rhs) {
	return !AstValue::operator==(rhs);
}

inline bool AstValue::operator<(const AstValue &rhs) {
	if (type() != STRING || rhs.type() != STRING) {
		return toReal() < rhs.toReal();
	}
	return toString() < rhs.toString();
}

inline bool AstValue::operator<=(const AstValue &rhs) {
	if (type() != STRING || rhs.type() != STRING) {
		return toReal() <= rhs.toReal();
	}
	return toString() <= rhs.toString();
}

inline bool AstValue::operator>(const AstValue &rhs) {
	return !operator<=(rhs);
}

inline bool AstValue::operator>=(const AstValue &rhs) {
	return !operator<(rhs);
}
