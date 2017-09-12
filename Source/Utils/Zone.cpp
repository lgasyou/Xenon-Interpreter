#include "Zone.h"
#include <algorithm>

Zone::~Zone() {
	deleteAll();
}

void Zone::addObject(ZoneObject *obj) {
	objects_.push_back(obj);
}

void Zone::deleteAll() {
	collecting_garbage_ = true;
	for (ZoneObject *obj : objects_) {
		delete obj;
	}
	objects_.clear();
}


Zone zoneInstance;

// static
void *ZoneObject::operator new(std::size_t size) {
	void *obj = malloc(size);
	zoneInstance.addObject(static_cast<ZoneObject *>(obj));
	return obj;
}

// static
void ZoneObject::operator delete(void *block) noexcept {
	if (!zoneInstance.collecting_garbage_) {
		auto beg = zoneInstance.objects_.begin();
		auto end = zoneInstance.objects_.end();
		auto iter = std::find(beg, end, static_cast<ZoneObject *>(block));
		*iter = nullptr;
	}
	free(block);
}
