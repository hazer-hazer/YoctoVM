#ifndef DATAOBJECT_H
#define DATAOBJECT_H

#include "core/Object.h"
#include "core/Function.h"

struct NExpression;

class Int;
class Float;
class Bool;
class String;

class TreeVisitor;
class Scope;

class DataObject : public Object, public Scope {
public:
	DataObject();
	virtual ~DataObject() = default;

	virtual Function * extend_method(const std::string & name, Function * func){
		return define_func(name, func);
	}

	virtual DataObject * call_method(TreeVisitor & visitor,
									 const std::string & name,
									 const Arguments & args)
	{
		return lookup_func(name)->apply(visitor, args);
	}

	// Extension methods
	virtual Bool * toBool() = 0;
	virtual String * toString() = 0;
};

#endif