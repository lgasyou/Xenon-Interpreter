#pragma once

#include "Stable.h"
#include <string>
#include "Utils/Zone.h"

// The AST refers to variables via VariableProxies.
// do not directly refer Variable object from the AST.
class Variable : public ZoneObject {
public:
	Variable(const std::string &name)
		: name_(name) {}

	const std::string &name() const { return name_; }

private:
	std::string name_;
};
