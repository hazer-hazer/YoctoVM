#ifndef DATAOBJECT_H
#define DATAOBJECT_H

#include "core/Object.h"

class DataObject : public Object {
public:
	DataObject() {}
	virtual ~DataObject() = default;

	virtual std::string to_string() override {
		return "DataObject";
	}
};

#endif