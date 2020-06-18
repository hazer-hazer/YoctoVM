#include "core/DataObject.h"

#include "core/Int.h"
#include "core/Float.h"
#include "core/Bool.h"
#include "core/String.h"

DataObject::DataObject(){
	// Extension functions list
	extend_method("toBool", new BuiltinFunction(std::vector<std::string>{}, [&](BuiltinFuncArgs){
		return toBool();
	}));
}