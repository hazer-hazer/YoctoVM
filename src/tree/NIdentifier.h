#ifndef NIDENTIFIER_H
#define NIDENTIFIER_H

#include "tree/Node.h"

struct NIdentifier : NExpression {
	Token name;

	NIdentifier(const Token & name) : name(name) {}
	virtual ~NIdentifier() = default;

	bool compare(NIdentifier & id){
		return id.name.String() == name.String();
	}

	std::string get_name(){
		return name.String();
	}

	virtual std::string to_string() override {
		return name.String();
	}

	virtual void accept(TreeVisitor & visitor) override {
		visitor.visit(*this);
	}

	void error(const std::string & msg) override {
		name.error(msg);
	}
};

#endif