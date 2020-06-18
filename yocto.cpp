#include "src/Lexer.h"
#include "src/Parser.h"
#include "src/core/TreeVisitor.h"
#include "src/core/Function.h"

#include "src/core/Int.h"
#include "src/core/String.h"

#include <fstream>
#include <iostream>
#include <exception>

int main(){
	std::fstream script_file("script.yo");

	try{

		if(!script_file.is_open()){
			throw "Unable to open file";
		}

		Lexer lexer(script_file);
		std::cout << "Lexing...\n";
		TokenStream tokens = lexer.lex();

		std::cout << "Tokens:\n";
		for(auto & t : tokens){
			std::cout << t.to_string() << std::endl;
		}

		Parser parser(tokens);
		std::cout << "Parsing...\n";
		ParseTree tree = parser.parse();

		std::cout << "\nTree:\n";
		for(const auto & n : tree){
			std::cout << n->to_string() << std::endl;
		}

		std::cout << "\nEvaluate...\n";

		TreeVisitor visitor;

		std::vector<std::string> params { "o" };
		BuiltinFunction * print = new BuiltinFunction(params, [&](BuiltinFuncArgs args){
			std::cout << args["o"]->toString()->get_value() << std::endl;
			return nullptr;
		});

		// Before evaluation started visitor scope is global scope
		visitor.get_scope()->define_func("print", print);

		visitor.visit_tree(tree);

	}catch(const char * e){
		std::cout << e << std::endl;
	}catch(const std::string & e){
		std::cout << e << std::endl;
	}catch(std::exception & e){
		std::cout << e.what() << std::endl;
	}

	return 0;
}