#include "core/Function.h"
#include "core/DataObject.h"
#include "core/Scope.h"

DataObject * UserFunction::apply(TreeVisitor & visitor, const Arguments & args){
	for(int i = 0; i < args.size(); i++){
		// Eval argument and put its value to function scope
		args[i]->accept(visitor);
		visitor.get_scope()->define_var(func_decl.params[i].get_name(), visitor.get<DataObject>());
	}

	func_decl.body.accept(visitor);

	return nullptr;
}

DataObject * BuiltinFunction::apply(TreeVisitor & visitor, const Arguments & args){
	std::map<std::string, DataObject*> builtin_args;

	for(int i = 0; i < args.size(); i++){
		args[i]->accept(visitor);
		builtin_args.insert({ params[i], visitor.get<DataObject>() });
		// visitor.get_scope()->define_var(params[i], visitor.get<DataObject>());
	}

	return body(builtin_args);
}