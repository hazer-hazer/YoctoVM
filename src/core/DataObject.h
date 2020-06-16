#ifndef DATAOBJECT_H
#define DATAOBJECT_H

#include "core/Object.h"
#include "core/Scope.h"
#include "core/TreeVisitor.h"
#include "core/Function.h"

struct NExpression;

class DataObject : public Object, public Scope {
public:
	DataObject() {}
	virtual ~DataObject() = default;

	virtual std::string to_string() = 0;

	virtual Function * extend_method(const std::string & name, Function * func){
		return define_func(name, func);
	}

	virtual DataObject * call_method(TreeVisitor & visitor,
									 const std::string & name,
									 const Arguments & args)
	{
		return lookup_func(name)->apply(visitor, args);
	}
};

#endif