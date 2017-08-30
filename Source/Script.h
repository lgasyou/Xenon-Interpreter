#pragma once

#include "Stable.h"
#include <string>

class Script {
public:
	static Script Compile(const std::string &source);

	static std::string Run();
};