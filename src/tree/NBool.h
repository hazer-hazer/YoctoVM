#ifndef NBOOL_H
#define NBOOL_H

#include "tree/Node.h"

struct NBool : NExpression {
	Token value;

	NBool(const Token & value) : value(value){};
	virtual ~NBool() = default;

	virtual std::string to_string() override {
		return std::to_string(value.Bool());
	}

	virtual void accept(TreeVisitor & visitor) override {
		visitor.visit(*this);
	}

	void error(const std::string & msg) override {
		value.error(msg);
	}
};

#endif