#pragma once

#include "Stable.h"
#include <string>
#include "Utils/Zone.h"

class Variable : public ZoneObject {
public:
	Variable(const std::string &name)
		: name_(name) {}

	const std::string &name() const { return name_; }

private:
	std::string name_;
};
