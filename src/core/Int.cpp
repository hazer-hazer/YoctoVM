#include "core/Int.h"

// #include "core/Function.h"
#include "core/Bool.h"
#include "core/String.h"

Int::Int(Token & token){
	if(token.type != T_INT){
		token.error("Expected int");
	}
	value = token.Int();

	// Extension methods

	// std::vector<std::string> __add_params { "another" };		
	// extend_method("__add", new BuiltinFunction(__add_params, [&](BuiltinFuncArgs args){
	// 	Int * another = dynamic_cast<Int*>(args["another"]);
	// 	if(!another){
	// 		throw "Expected Int";
	// 	}
	// 	return new Int(another->get_value() + value);
	// }));
}

int Int::get_value(){
	return value;
}

Bool * Int::toBool(){
	return new Bool(static_cast<bool>(value));
}

String * Int::toString(){
	return new String(std::to_string(value));
}