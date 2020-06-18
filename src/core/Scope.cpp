#include "core/Scope.h"

#include "core/DataObject.h"
#include "core/Function.h"
#include "tree/NIdentifier.h"

Scope::Scope(Scope * parent){
	set_parent(parent);
}

Scope * Scope::get_parent(){
	return parent;
}

void Scope::set_parent(Scope * parent){
	this->parent = parent;
}

void Scope::assert_name_available(NIdentifier & id){
	if(locals.find(id.get_name()) != locals.end()
	|| functions.find(id.get_name()) != functions.end())
	{
		id.error("is already defined");
	}
}

DataObject * Scope::define_var(NIdentifier & id, DataObject * value){
	assert_name_available(id.get_name());
	return locals[id.get_name()] = value;
}

DataObject * Scope::assign_var(NIdentifier & id, DataObject * value){
	if(locals.find(id.get_name()) == locals.end()){
		id.error("is not defined");
	}
	return locals[id.get_name()] = value;
}

DataObject * Scope::lookup_var(NIdentifier & id){
	if(locals.find(id.get_name()) != locals.end()){
		return locals[id.get_name()];
	}else if(parent){
		return parent->lookup_var(id.get_name());
	}else{
		id.error("is not defined");
		return nullptr;
	}
}

Function * Scope::define_func(NIdentifier & id, Function * func){
	// TODO: Rewrite after types implementation
	// Now it's not possible to overload function
	
	assert_name_available(id.get_name());

	return functions[id.get_name()] = func;

	// // Note: It's not possible to define function with name 
	// // that is already taken for variable
	
	// if(locals.find(name) != locals.end()){
	// 	throw name + " is already defined";
	// }

	// // Note: Now there's no reason to make `functions` multimap
	// // because here's no types
	// // But I do it now, because want to implement types in the future

	// // Find all functions with same name
	// // and check if at least one of them has the same structure
	// const auto range = functions.equal_range(name);
	// for(auto i = range.first; i != range.second; i++){
	// 	if(i->second->compare(func_decl)){
	// 		throw name + ": function with this signature already defined";
	// 		return nullptr;
	// 	}
	// }

	// functions[name] = func_decl;
}

Function * Scope::lookup_func(NIdentifier & id){
	if(functions.find(id.get_name()) != functions.end()){
		return functions[id.get_name()];
	}else if(parent){
		return parent->lookup_func(id.get_name());
	}
	id.error("is not defined");
}

// Debug
std::string Scope::to_string(){
	return "";
	// std::string str;
	// str += "Locals defined:\n";
	// for(const auto & l : locals){
	// 	str += l.first + " = " + l.second->to_string() + "\n";
	// }
	// str += "\nFunctions defined:\n";
	// for(const auto & f : functions){
	// 	str += f.first + " = " + f.second->to_string() + "\n";
	// }
	// return str;
}