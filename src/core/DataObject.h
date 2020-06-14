#ifndef DATAOBJECT_H
#define DATAOBJECT_H

#include "core/Object.h"

class DataObject : public Object {
public:
	DataObject() {}
	virtual ~DataObject() = default;
};

#endif