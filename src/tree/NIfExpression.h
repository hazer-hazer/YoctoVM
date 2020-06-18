#ifndef NIF_H
#define NIF_H

#include "tree/Node.h"

// Note: ConditionStructure cannot have null condition or null body
// so they're references
struct ConditionStructure {
	NExpression & condition;
	NBlock & body;

	ConditionStructure(NExpression & condition, NBlock & body)
		: condition(condition), body(body) {}
	virtual ~ConditionStructure() = default;

	std::string to_string() const {
		return condition.to_string() + " " + body.to_string();
	}
};

typedef std::vector<ConditionStructure> ConditionList;

inline std::string conditions_to_str(const ConditionList & conditions){
	std::string str;
	bool first = true;
	for(const ConditionStructure & c : conditions){
		if(first){
			str += "if ";
			first = false;
		}else{
			str += " elif ";
		}
		str += c.to_string();
	}
	return str;
}

struct NIfExpression : NExpression {
	// Note: There's no separate field for If.
	// Evaluator checks each condition starting from first
	// or evals Else if it exists
	ConditionList conditions;
	NBlock * Else;

	NIfExpression(const ConditionList & conditions, NBlock * Else)
		: conditions(conditions), Else(Else) {}
	virtual ~NIfExpression() = default;

	virtual std::string to_string() override {
		return conditions_to_str(conditions) + (Else ? " else " + Else->to_string() : "");
	}

	virtual void accept(TreeVisitor & visitor) override {
		visitor.visit(*this);
	}

	void error(const std::string & msg) override {
		if(conditions.size() > 0){
			conditions[0].condition.error(msg);
		}
	}
};

#endif