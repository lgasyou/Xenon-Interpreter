#pragma once
#include <exception>

class Exception : public std::exception {
public:
	Exception();
	~Exception();
};

