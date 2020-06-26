#ifndef INT_H
#define INT_H

#include "Object/DataObject.h"

class Int : public DataObject {
public:
	Int(const int & i);
	virtual ~Int() = default;

	Bool * toBool() override;
	String * toString() override;

	int get_val() const {
		return value;
	}

private:
	int value;
};

#endif