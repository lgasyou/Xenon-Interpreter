#pragma once

#include "Stable.h"
#include <vector>

class ZoneObject;

/* This class manages all ZoneObjects.
 * Will call deleteAll() automatically when program is over.
**/
class Zone {
public:
	Zone() = default;
	~Zone();

	void addObject(ZoneObject *obj);

	void deleteAll();

private:
	std::vector<ZoneObject *> objects_;
};


class ZoneObject {
public:
	static void *operator new(std::size_t size);
};
