#ifndef NFUNCDECL_H
#define NFUNCDECL_H

#include "tree/Node.h"
#include "tree/NIdentifier.h"
#include "tree/NBlock.h"

struct Param {
	NIdentifier & id;

	Param(NIdentifier & id) : id(id) {}
	virtual ~Param() = default;

	std::string get_name(){
		return id.get_name();
	}
};

// TODO!: Add special functions like get name, etc. for Nodes there's need

struct NFuncDecl : NStatement {
	NIdentifier & id;
	std::vector<Param> params;
	NBlock & body;

	NFuncDecl(NIdentifier & id, const std::vector<Param> & params, NBlock & body)
		: id(id), params(params), body(body) {}
	virtual ~NFuncDecl() = default;

	std::string get_name(){
		return id.get_name();
	}

	bool compare(NFuncDecl & func_decl){
		// TODO: Change params comparison for types
		return func_decl.id.compare(id)
			&& func_decl.params.size() == params.size();
	}

	virtual std::string to_string() override {
		std::string params_str;
		for(int i = 0; i < params.size(); i++){
			params_str += params[i].id.to_string();
			if(i < params.size() - 1){
				params_str += ", ";
			}
		}
		return "func " + id.to_string() + "(" + params_str + ")" + body.to_string();
	}

	virtual void accept(TreeVisitor & visitor) override {
		visitor.visit(*this);
	}

	void error(const std::string & msg) override {
		id.error(msg);
	}
};

#endif