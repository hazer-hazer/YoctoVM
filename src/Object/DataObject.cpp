#include "Object/DataObject.h"
#include "Object/Bool.h"
#include "Object/String.h"

DataObject::DataObject(){}

Bool * DataObject::toBool(){
	return new Bool(true);
}

String * DataObject::toString(){
	return new String("Object");
}