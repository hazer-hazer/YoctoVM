#include "src/Lexer.h"
#include "src/Parser.h"
#include "src/core/TreeVisitor.h"

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
		for(Node * n : tree){
			std::cout << n->to_string() << std::endl;
		}

		std::cout << "\nEvaluate...\n";

		TreeVisitor visitor;

		for(Node * n : tree){
			if(n){
				visitor.visit(n);
			}
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