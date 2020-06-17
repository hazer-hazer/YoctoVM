#include "core/Scope.h"

#include "core/DataObject.h"
#include "core/Function.h"

Scope::Scope(Scope * parent){
	this->parent = parent;
}

Scope * Scope::get_parent(){
	return parent;
}

void Scope::assert_name_available(const std::string & name){
	if(locals.find(name) != locals.end()
	|| functions.find(name) != functions.end()
	|| types.find(name) != types.end())
	{
		throw name + " is already defined";
	}
}

DataObject * Scope::define_var(const std::string & name, DataObject * value){
	assert_name_available(name);
	return locals[name] = value;
}

DataObject * Scope::assign_var(const std::string & name, DataObject * value){
	if(locals.find(name) != locals.end()){
		throw name + " is not defined";
	}
	return locals[name] = value;
}

DataObject * Scope::lookup_var(const std::string & name){
	if(locals.find(name) != locals.end()){
		return locals[name];
	}else if(parent){
		return parent->lookup_var(name);
	}else{
		throw name + " is not defined";
		return nullptr;
	}
}

Function * Scope::define_func(const std::string & name, Function * func){
	// TODO: Rewrite after types implementation
	// Now it's not possible to overload function
	
	assert_name_available(name);

	return functions[name] = func;

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

Function * Scope::lookup_func(const std::string & name){
	if(functions.find(name) != functions.end()){
		return functions[name];
	}else if(parent){
		return parent->lookup_func(name);
	}
	throw name + " is not defined";
}

NType * Scope::define_type(const std::string & name, NType * type){
	assert_name_available(name);

	return types[name] = type;
}

NType * Scope::lookup_type(const std::string & name){
	if(types.find(name) != types.end()){
		return types[name];
	}else if(parent){
		return parent->lookup_type(name);
	}
	throw "type "+ name +" is not defined";
}

// Debug
std::string Scope::to_string(){
	std::string str;
	str += "Locals defined:\n";
	for(const auto & l : locals){
		str += l.first + " = " + l.second->to_string() + "\n";
	}
	str += "\nFunctions defined:\n";
	for(const auto & f : functions){
		str += f.first + " = " + f.second->to_string() + "\n";
	}
	return str;
}