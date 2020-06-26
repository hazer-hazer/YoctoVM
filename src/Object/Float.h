#ifndef FLOAT_H
#define FLOAT_H

#include "Object/DataObject.h"

class Float : public DataObject {
public:
	Float(const double & d);
	virtual ~Float() = default;

	Bool * toBool() override;
	String * toString() override;

	double get_val() const {
		return value;
	}

private:
	double value;
};

#endif