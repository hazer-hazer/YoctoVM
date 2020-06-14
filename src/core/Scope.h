#ifndef SCOPE_H
#define SCOPE_H

#include <map>
#include <string>

#include "core/Object.h"

class Scope {
public:
	// Scope(Scope * parent = nullptr) {
	// 	this->parent = parent;
	// }
	// virtual ~Scope() = default;

	// bool contains(const std::string & name){
	// 	return locals.find(name) != locals.end();
	// }

	// Object * define(NIdentifier & id, Object * value){
	// 	std::string name = id.id.String();
	// 	if(contains(name)){
	// 		throw "Unable to redefine variable " + name;
	// 	}
	// 	return locals[name] = value;
	// }
	
	// Object * assign(NIdentifier & id, Object * value){
	// 	std::string name = id.id.String();
	// 	if(!contains(name)){
	// 		throw name + " is not defined";
	// 	}
	// 	return locals[name] = value;
	// }

	// Object * get(const std::string & name){
	// 	if(contains(name)){
	// 		return locals[name];
	// 	}else{
	// 		throw name + " is not defined";
	// 	}
	// }

	// Object * lookup(NIdentifier & id){
	// 	std::string name = id.id.String();
	// 	if(contains(name)){
	// 		return get(name);
	// 	}else if(parent){
	// 		return parent->lookup(id);
	// 	}else{
	// 		throw name + " is not defined";
	// 		return nullptr;
	// 	}
	// }

private:
	Scope * parent;

	std::map<std::string, Object*> locals;
};

#endif