#ifndef NVARDECL_H
#define NVARDECL_H

#include "tree/Node.h"

struct NVarDecl : NStatement {
	NVarDecl(){}
	virtual ~NVarDecl() = default;

	virtual std::string to_string() override {
		return "[NVarDecl]";
	}
};

#endif