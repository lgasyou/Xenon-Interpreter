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