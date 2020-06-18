#ifndef SCOPE_H
#define SCOPE_H

#include <map>
#include <string>

// Note: The reason why Scope functions receive NIdentifier and not a string
// is because of error tracking

class Object;
class DataObject;
class Function;

struct NIdentifier;

class Scope {
public:
	Scope(Scope * parent = nullptr);
	virtual ~Scope() = default;

	Scope * get_parent();
	void set_parent(Scope * parent);

	void assert_name_available(NIdentifier & id);
	DataObject * define_var(NIdentifier & id, DataObject * value);
	DataObject * assign_var(NIdentifier & id, DataObject * value);
	DataObject * lookup_var(NIdentifier & id);

	Function * define_func(NIdentifier & id, Function * func);
	Function * lookup_func(NIdentifier & id);

	// Debug
	std::string to_string();

private:
	Scope * parent;

	std::map<std::string, DataObject*> locals;
	std::map<std::string, Function*> functions;
};

#endif