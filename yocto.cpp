#include "src/Lexer.h"
#include "src/Parser.h"

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
		TokenStream tokens = lexer.lex();

		std::cout << "Tokens:\n";
		for(auto & t : tokens){
			std::cout << t.to_string() << std::endl;
		}

		Parser parser(tokens);
		ParseTree tree = parser.parse();

		std::cout << "\nTree:\n";
		for(Node * n : tree){
			std::cout << n->to_string() << std::endl;
		}

		Scope * global = new Scope();

		for(Node * n : tree){
			n->eval(global);
		}

	}catch(const char * e){
		std::cout << e << std::endl;
	}catch(std::exception & e){
		std::cout << e.what() << std::endl;
	}catch(const std::string & e){
		std::cout << e << std::endl;
	}

	return 0;
}