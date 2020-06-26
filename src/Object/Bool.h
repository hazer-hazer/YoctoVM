#ifndef BOOL_H
#define BOOL_H

#include "Object/DataObject.h"

class Bool : public DataObject {
public:
	Bool(const bool & b);
	virtual ~Bool() = default;

	Bool * toBool() override;
	String * toString() override;

	bool get_val() const {
		return value;
	}

private:
	bool value;
};

#endif