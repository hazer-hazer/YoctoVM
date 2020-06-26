#include "Yocto.h"

Yocto::Yocto(){}

Yocto::~Yocto(){}

void Yocto::run_prompt(){
	while(!std::cin.eof()){
		std::cout << "> ";
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
	const auto bench = std::chrono::high_resolution_clock::now();

	// std::cout << "Run script:\n" << script << std::endl;

	// Lexing
	Lexer lexer(script);
	auto lexer_start = bench;
	TokenStream tokens = lexer.lex();
	auto lexer_end = bench;

	std::cout << "Tokens:\n";
	for(auto & t : tokens){
		std::cout << t.to_string() << std::endl;
	}

	// Parse tokens
	Parser parser(tokens);
	auto parser_start = bench;
	ParseTree tree = parser.parse();
	auto parser_end = bench;

	// Print tree
	ParseTreePrinter printer;
	std::cout << "\nParse Tree:\n";
	printer.visit(tree);
	std::cout << std::endl;

	// Compile AST
	std::cout << "Compile...\n";
	auto chunk = Chunk();
	Compiler compiler(chunk);
	auto compiler_start = bench;
	compiler.visit(tree);
	auto compiler_end = bench;

	// Run bytecode
	VM vm(chunk);
	auto vm_start = bench;
	vm.run();
	auto vm_end = bench;

	std::cout << "\n\nBenchmarks:\n";

	auto lexer_duration = std::chrono::duration<double>(lexer_end - lexer_start).count();
	std::cout << "Lexing was done in: " << lexer_duration << "s" << std::endl;

	auto parser_duration = std::chrono::duration<double>(parser_end - parser_start).count();
	std::cout << "Parsing was done in: " << parser_duration << "s" << std::endl;

	auto compiler_duration = std::chrono::duration<double>(compiler_end - compiler_start).count();
	std::cout << "Compilation was done in: " << compiler_duration << "s" << std::endl;

	auto vm_duration = std::chrono::duration<double>(vm_end - vm_start).count();
	std::cout << "Evaluated in: " << vm_duration << "s" << std::endl;
}

// Note: Benchmark for script size of 100000 lines was lexed in 0.36s