#ifndef OBJECT_H
#define OBJECT_H

#include <string>

class Object {
public:
	Object() {}
	virtual ~Object() = default;

	virtual std::string to_string(){
		return "Object";
	}

private:
};

#endif