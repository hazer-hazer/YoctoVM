#ifndef NIDENTIFIER_H
#define NIDENTIFIER_H

#include "tree/Node.h"

struct NIdentifier : NExpression {
	Token id;

	NIdentifier(const Token & id) : id(id) {}
	virtual ~NIdentifier() = default;

	virtual std::string to_string() override {
		return id.String();
	}

	virtual Object * visit(Scope * scope) override {
		if(!scope){
			throw "Unable to use variable";
		}
		return scope->lookup(id);
	}
};

#endif