#ifndef NBLOCK_H
#define NBLOCK_H

#include "tree/Node.h"

struct NBlock : NExpression {
	StatementList statements;

	NBlock(const StatementList & statements) : statements(statements) {}
	virtual ~NBlock() = default;

	virtual std::string to_string() override {
		return "{\n" + statements_to_str(statements) + "\n}";
	}

	virtual Object * visit(Scope * scope) override {
		Object * value = nullptr;
		for(NStatement * stmt : statements){
			if(stmt){
				value = stmt->eval(scope);
			}
		}
		return value;
	}
};

#endif