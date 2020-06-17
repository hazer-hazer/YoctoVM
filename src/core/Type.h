#ifndef TYPE_H
#define TYPE_H

#include "tree/NType.h"

class Type {
public:
	Type(NType & node) : node(node) {}
	virtual ~Type() = default;

	virtual bool compare(Type & type){
		return type.get_node().compare(node);
	}

	NType & get_node(){
		return node;
	}

private:
	NType & node;
};

#endif