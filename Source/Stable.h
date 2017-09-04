#pragma once

#include <iostream>
#include <cstdint>
#include <cstring>

#define DEBUG		1
#define UNIT_TEST	0

#if DEBUG
#define DBG_PRINT	std::cout
#else 
#define DBG_PRINT	0 && std::cout
#endif // DEBUG

#define UNREACHABLE()																\
	do {																			\
		printf("File: %s, Func: %s, Line: %d\n", __FILE__, __FUNCTION__, __LINE__);	\
		abort();																	\
	} while (false)

