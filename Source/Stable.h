#pragma once

#include <iostream>
#include <cstdint>
#include <cstring>

#define DEBUG		1
#define UNIT_TEST	0

#if DEBUG
// A macro which is an aligas for std::cout when DEBUG == 1, 
// otherwise it won't do anything. Use it as the same as std::cout.
// Use like:
//	DBG_PRINT << "hello, world";
#define DBG_PRINT	std::cout

// A macro used to print info and then call abort() 
// when this program steps into a unreachable place.
// Use like:
//	UNREACHABLE();
#define UNREACHABLE()																\
	do {																			\
		printf("File: %s, Func: %s, Line: %d\n", __FILE__, __FUNCTION__, __LINE__);	\
		abort();																	\
	} while (false)

#else 
// A macro which is an aligas for std::cout when DEBUG == 1, 
// otherwise it won't do anything. Use it as the same as std::cout.
// Use like:
//	DBG_PRINT << "hello, world";
#define DBG_PRINT	0 && std::cout

// A macro used to print info and call abort() 
// when this program steps into a unreachable place.
// Use like:
//	UNREACHABLE();
#define UNREACHABLE() /* NOT SUPPORTED */

#endif // DEBUG

template <typename T>
bool FirstIsOneOf(T target, T last) {
	return target == last;
}

template <typename T, typename... Args>
bool FirstIsOneOf(T target, T first, Args... others) {
	return target == first || FirstIsOneOf(target, others...);
}
