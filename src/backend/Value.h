#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <variant>

#include "Object/Object.h"
#include "Object/DataObject.h"
#include "Object/String.h"
#include "Object/Bool.h"
#include "Object/Int.h"
#include "Object/Float.h"

#define FLOAT_NUM_PRECISION 15

enum class ValueType {
	Null,
	DataObj,
	Function
};

struct Value {
	ValueType type;
	Object * obj;
};

inline std::ostream & operator<<(std::ostream & os, const Value & val){
	switch(val.type){
		case ValueType::Null:{
			os << "null";
			break;
		}
		case ValueType::DataObj:{
			os << static_cast<DataObject*>(val.obj)->toString()->get_val();
		}
		case ValueType::Function:{
			os << "function";
		}
	}
	return os;
}

#endif