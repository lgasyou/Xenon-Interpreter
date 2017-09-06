#pragma once

#include "Stable.h"

class AstValue;

class MCObject {

};

class MCFunction : public MCObject {
public:
	//MCFunction(const std::vector<AstValue> &args);

	AstValue operator()();
};
