#ifndef SCOPE_H
#define SCOPE_H

#include <map>
#include <string>

class Object;
class DataObject;
class Function;

class Scope {
public:
	Scope(Scope * parent = nullptr);
	virtual ~Scope() = default;

	Scope * get_parent();

	void assert_name_available(const std::string & name);
	DataObject * define_var(const std::string & name, DataObject * value);
	DataObject * assign_var(const std::string & name, DataObject * value);
	DataObject * lookup_var(const std::string & name);

	Function * define_func(const std::string & name, Function * func);
	Function * lookup_func(const std::string & name);

	NType * define_type(const std::string & name, NType * type);
	NType * lookup_type(const std::string & name);

	// Debug
	std::string to_string();

private:
	Scope * parent;

	std::map<std::string, DataObject*> locals;
	std::map<std::string, Function*> functions;
	std::map<std::string, NType*> types;
};

#endif