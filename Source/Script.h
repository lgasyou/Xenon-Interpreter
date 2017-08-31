#pragma once

#include "Stable.h"
#include <string>

class AstNode;

class Script {
public:
	Script(AstNode *root);

	static Script Compile(const std::string &source);

	void Run();

private:
	AstNode *root_;
};
