#include "Variable.h"

Variable::Variable(int val) {
	type_ = INTEGER;
	var.integer = val;
}

Variable::Variable(float val) {
	type_ = REAL;
	var.real = val;
}

Variable::Variable(const std::string &val) {
	type_ = STRING;
	var.string = new std::string(val);
}

Variable::~Variable() {
	if (type() == STRING) {
		delete var.string;
	}
}

int Variable::toInt() {
	return var.integer;
}

float Variable::toReal() {
	return var.real;
}

std::string Variable::toString() {
	return *var.string;
}

#include "Utils/UnitTest.h"
TEST_CASE(Variable) {
	auto i = Variable(10);
	DBG_PRINT << i.type() << "\n";
	DBG_PRINT << i.toInt() << "\n";

	auto r = Variable(20.5f);
	DBG_PRINT << r.type() << "\n";
	DBG_PRINT << r.toReal() << "\n";

	std::string str = "Hello";
	auto s = Variable(str);
	DBG_PRINT << s.type() << "\n";
	DBG_PRINT << s.toString() << "\n";
}
