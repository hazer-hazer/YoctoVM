#ifndef NFUNCCALL_H
#define NFUNCCALL_H

#include "tree/Node.h"

struct NFuncCall : NExpression {
	NExpression & left;
	ExpressionList args;

	NFuncCall(NExpression & left, const ExpressionList & args)
		: left(left), args(args) {}

	virtual std::string to_string() override {
		std::string args_str;
		for(int i = 0; i < args.size(); i++){
			args_str += args[i]->to_string();
			if(i < args.size() - 1){
				args_str += ", ";
			}
		}
		return left.to_string() + "(" + args_str + ")";
	}

	virtual void accept(TreeVisitor & visitor) override {
		visitor.visit(*this);
	}

	void error(const std::string & msg) override {
		left.error(msg);
	}
};

#endif