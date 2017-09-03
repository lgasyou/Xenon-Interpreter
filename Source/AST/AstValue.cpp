#include "AstValue.h"

AstValue::AstValue(int val) {
	type_ = INTEGER;
	var.integer = val;
}

AstValue::AstValue(float val) {
	type_ = REAL;
	var.real = val;
}

AstValue::AstValue(const std::string &val) {
	type_ = STRING;
	var.string = new std::string(val);
}

AstValue::AstValue(const AstValue &rhs) {
	type_ = rhs.type_;
	if (type() == INTEGER) {
		var.integer = rhs.var.integer;
	} else if (type() == REAL) {
		var.real = rhs.var.real;
	} else {
		var.string = new std::string(*rhs.var.string);
	}
}

AstValue::~AstValue() {
	if (type() == STRING) {
		delete var.string;
	}
}

int AstValue::toInt() const {
	return var.integer;
}

float AstValue::toReal() const {
	return var.real;
}

const std::string &AstValue::toString() const {
	return *var.string;
}