#pragma once

#include "Stable.h"
#include "Utils/Zone.h"

class AstValue;

class MCObject : public ZoneObject {
public:

};

class MCFunction : public MCObject {
public:
	enum Type {
		INTEGER,
		REAL,
		STRING,
	};

	struct Argument {
		MCFunction::Type type;
		std::string name;
	};

	MCFunction(const std::vector<Argument> &actualArguments)
		: actual_arguments_(actualArguments) {}

	AstValue operator()(const std::vector<AstValue> &formalArguments);

private:
	std::vector<Argument> actual_arguments_;
};
