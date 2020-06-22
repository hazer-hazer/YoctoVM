#include "backend/Value.h"

ValueArray::ValueArray(){}

void ValueArray::write(Value val){
	values.push_back(val);
}