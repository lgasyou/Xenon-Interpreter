#pragma once

#include "Stable.h"
#include <vector>

class ZoneObject;

/* This class manages all ZoneObjects.
 * Will call deleteAll() automatically when program is over.
**/
class Zone {
	friend class ZoneObject;
public:
	Zone() = default;
	~Zone();

	void addObject(ZoneObject *obj);

	// Deletes all ZoneoObjects which're added by addObject(ZoneObject *obj).
	void deleteAll();

private:
	std::vector<ZoneObject *> objects_;
};


/* ZoneObject is an abstraction that helps define classes of objects
 * allocated in the Zone. Use it as a base class.
**/
class ZoneObject {
public:
	virtual ~ZoneObject() {}

	static void *operator new(std::size_t size);
	static void operator delete(void *) noexcept;
};
