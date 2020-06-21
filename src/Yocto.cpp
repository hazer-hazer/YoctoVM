#include "Yocto.h"

Yocto::Yocto(){}

Yocto::~Yocto(){}

void Yocto::run_prompt(){
	while(!std::cin.eof()){
		std::cout << "Yo> ";
		std::string line;
		std::getline(std::cin, line);
		run(line);
		std::cout << "\n";
	}
}

void Yocto::run_script(const std::string & path){
	std::fstream file(path);

	if(!file.is_open()){
		throw FileNotFoundException(path);
	}

	std::stringstream ss;
	ss << file.rdbuf();
	std::string script = ss.str();

	run(script);

	file.close();
}

void Yocto::run(const std::string & script){	
	// std::cout << "Run script:\n" << script << std::endl;

	Lexer lexer(script);

	auto lexer_start = std::chrono::high_resolution_clock::now();
	TokenStream tokens = lexer.lex();
	auto lexer_end = std::chrono::high_resolution_clock::now();

	std::cout << "Tokens:\n";
	for(auto & t : tokens){
		std::cout << t.to_string() << std::endl;
	}

	Parser parser(tokens);

	auto parser_start = std::chrono::high_resolution_clock::now();
	ParseTree tree = parser.parse();
	auto parser_end = std::chrono::high_resolution_clock::now();

	ParseTreePrinter printer;

	std::cout << "\nParse Tree:\n";
	printer.visit(tree);

	std::cout << std::endl;
	
	auto lexer_duration = std::chrono::duration<double>(lexer_end - lexer_start).count();
	std::cout << "Lexing was done in: " << lexer_duration << "s" << std::endl;

	auto parser_duration = std::chrono::duration<double>(parser_end - parser_start).count();
	std::cout << "Parsing was done in: " << parser_duration << "s" << std::endl;
}

// Note: Benchmark for script size of 100000 lines was lexed in 0.36s