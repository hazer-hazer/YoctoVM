#include "backend/Value.h"

ValueArray::ValueArray(){
	init();
}

void ValueArray::init(){
	capacity = 0;
	count = 0;
	values = nullptr;
}

void ValueArray::clear(){
	FREE_ARRAY(Value, values, capacity);
	init();
}

void ValueArray::write(Value val){
	if(capacity < count + 1){
		int oldCapacity = capacity;
		capacity = GROW_CAPACITY(oldCapacity);
		values = GROW_ARRAY(values, Value, oldCapacity, capacity);
	}
	values[count] = val;
	count++;
}