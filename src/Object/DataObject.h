#ifndef DATAOBJECT_H
#define DATAOBJECT_H

#include "Object/Object.h"

class Int;
class Float;
class Bool;
class String;

class DataObject : public Object {
public:
	DataObject();
	virtual ~DataObject() = default;

	virtual Bool * toBool();
	virtual String * toString();
};

#endif