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
	return type() == REAL ? var.real : var.integer;
}

const std::string &AstValue::toString() const {
	return *var.string;
}

static AstValue StringDelete(const AstValue &lhs, const AstValue &rhs) {
	char target = rhs.toString()[0];
	auto newString = lhs.toString();
	auto iter = std::find(newString.begin(), newString.end(), target);
	if (iter != newString.end()) {
		newString.erase(iter);
	}
	return AstValue(newString);
}

AstValue AstValue::operator-(const AstValue &rhs) {
	if (type() == REAL || rhs.type() == REAL) {
		return AstValue(toReal() - rhs.toReal());
	} else if (type() == INTEGER || rhs.type() == INTEGER) {
		return AstValue(toInt() - rhs.toInt());
	}
	return AstValue(StringDelete(*this, rhs));
}


#include "Utils/UnitTest.h"
TEST_CASE(AstValue) {
	ASSERT_EQ(AstValue(30), AstValue(10) + AstValue(20));
	ASSERT_EQ(AstValue(30.5f), AstValue(10.5f) + AstValue(20));
	ASSERT_EQ(AstValue(40.0f), AstValue(20.0f) + AstValue(20.0f));
	ASSERT_EQ(AstValue("Hello"), AstValue("Hello"));

	ASSERT_EQ(false, !AstValue(40));
	ASSERT_EQ(true, !AstValue(0));

	//ASSERT_NE(AstValue(20.0f), AstValue(20.0f));
	ASSERT_NE(AstValue(18.0f), AstValue(20.0f));
	ASSERT_NE(AstValue(5), AstValue(10));
	ASSERT_NE(AstValue("Hello"), AstValue("Hello, world"));
	//ASSERT_NE(AstValue("Hello"), AstValue("Hello"));

	ASSERT_LTE(AstValue(20.0f), AstValue(20.0f));
	ASSERT_LTE(AstValue(18.0f), AstValue(20.0f));
	ASSERT_LTE(AstValue(5), AstValue(10));
	ASSERT_LTE(AstValue("Hello"), AstValue("Hello, world"));
	ASSERT_LTE(AstValue("Hello"), AstValue("Hello"));

	//ASSERT_LT(AstValue(20.0f), AstValue(20.0f));
	ASSERT_LT(AstValue(18.0f), AstValue(20.0f));
	ASSERT_LT(AstValue(5), AstValue(10));
	ASSERT_LT(AstValue("Hello"), AstValue("Hello, world"));
	//ASSERT_LT(AstValue("Hello"), AstValue("Hello"));

	ASSERT_GTE(AstValue(35.0f), AstValue(20.0f));
	ASSERT_GTE(AstValue(30.0f), AstValue(10.5f));
	ASSERT_GTE(AstValue(30), AstValue(10));
	ASSERT_GTE(AstValue("Hello, world"), AstValue("Hello"));
	ASSERT_GTE(AstValue("Hello"), AstValue("Hello"));

	ASSERT_GT(AstValue(35.0f), AstValue(20.0f));
	ASSERT_GT(AstValue(30.0f), AstValue(10.5f));
	ASSERT_GT(AstValue(30), AstValue(10));
	ASSERT_GT(AstValue("Hello, world"), AstValue("Hello"));
	//ASSERT_GT(AstValue("Hello"), AstValue("Hello"));

	ASSERT_EQ(AstValue("Hello, world"), AstValue("Hello") + AstValue(", world"));
	ASSERT_EQ(AstValue("ello"), AstValue("hello") - AstValue("h"));
	ASSERT_EQ(AstValue("ol"), AstValue("lol") - AstValue("l"));
	ASSERT_EQ(AstValue("hello"), AstValue("hello") - AstValue("x"));
}
