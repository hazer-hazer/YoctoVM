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

	virtual void accept(TreeVisitor & visitor) override {
		visitor.visit(*this);
	}
};

#endif