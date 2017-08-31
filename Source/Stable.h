#pragma once

#include <iostream>
#include <cstdint>
#include <cstring>

#define DEBUG 1
#define UNIT_TEST 1

#if DEBUG
#define DBG_PRINT std::cout
#else 
#define DBG_PRINT 0 && std::cout
#endif // DEBUG
