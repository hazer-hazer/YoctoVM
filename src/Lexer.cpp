#include "Lexer.h"

Lexer::Lexer(const std::string & script){
	this->script = script;
	index = 0;
	line = 1;
	column = 1;
}

char Lexer::peek(){
	return script[index];
}

char Lexer::peekNext(){
	return script[index + 1];
}

char Lexer::advance(){
	index++;
	if(peek() == '\n'){
		line++;
		column = 1;
	}else{
		column++;
	}
	return peek();
}

bool Lexer::eof(){
	return peek() == '\0' || index >= script.size();
}

bool Lexer::skip(const char & c){
	return c == '\t' || c == ' ' || c == '\r';
}

bool Lexer::is_nl(const char & c){
	return c == '\n';
}

bool Lexer::is_digit(const char & c){
	return c >= '0' && c <= '9';
}

bool Lexer::is_id_first(const char & c){
	return (c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| (c == '_');
}

bool Lexer::is_id(const char & c){
	return is_id_first(c) || is_digit(c);
}

bool Lexer::is_quote(const char & c){
	return c == '"' || c == '\'' || c == '`';
}

void Lexer::add_token(Token t){
	t.pos.line = token_line;
	t.pos.column = token_column;

	tokens.push_back(t);
}

void Lexer::add_token(const TokenType & type, const std::string & val){
	add_token(Token(type, val));
}

void Lexer::add_token(const Operator & op){
	add_token(Token(op));
}

void Lexer::add_token(const Keyword & kw){
	add_token(Token(kw));
}

void Lexer::add_token(const TokenType & type){
	add_token(Token(type));
}

void Lexer::add_token(const int & i){
	add_token(Token(i));
}

void Lexer::add_token(const double & d){
	add_token(Token(d));
}

void Lexer::lex_number(){
	TokenType num_type = T_INT;
	std::string num;

	if(peek() == '0'){
		advance();
		switch(peek()){
			case 'x':
			case 'X':{
				advance();
				if(!is_digit(peek())){
					unexpected_error();
				}
				do{
					num += peek();
				}while(is_digit(advance()));

				add_token(std::stoi(num, 0, 16));
				return;
				break;
			}
			case 'b':
			case 'B':{
				advance();
				if(!is_digit(peek())){
					unexpected_error();
				}
				do{
					num += peek();
				}while(is_digit(advance()));

				add_token(std::stoi(num, 0, 2));
				return;
				break;
			}
			default: num = "0";
		}
	}

	while(is_digit(peek())){
		num += peek();
		advance();
	}

	if(peek() == '.'){
		num_type = T_FLOAT;
		num += peek();
		advance();
		if(!is_digit(peek())){
			unexpected_error();
		}
		do{
			num += peek();
		}while(is_digit(advance()));
	}

	if(num_type == T_FLOAT){
		add_token(std::stod(num));
	}else{
		add_token(std::stoi(num));
	}
}

TokenStream Lexer::lex(){

	while(!eof()){
		token_line = line;
		token_column = column;
	
		if(skip(peek())){
			advance();
		}else if(is_nl(peek())){
			add_token(T_NL);
			advance();
		}else if(is_digit(peek())){
			lex_number();
		}else if(is_id_first(peek())){
			std::string id(1, peek());
			while(is_id(advance())){
				id += peek();
			}

			Keyword kw = str_to_kw(id);

			if(kw < KW_MAX){
				add_token(kw);
			}else{
				add_token(T_ID, id);
			}
		}else if(is_quote(peek())){
			const char quote = peek();
			advance();
			std::string str = "";
			while(!eof()){
				if(peek() == quote){
					break;
				}
				str += peek();
				advance();
			}
			if(eof()){
				unexpected_eof_error();
			}
			if(peek() != quote){
				unexpected_error();
			}
			add_token(T_STR, str);
			advance();
		}else{
			switch(peek()){
				case '=':{
					add_token(OP_ASSIGN);
					advance();
					break;
				}
				case '+':{
					add_token(OP_ADD);
					advance();
					break;
				}
				case '-':{
					add_token(OP_SUB);
					advance();
					break;
				}
				case '*':{
					add_token(OP_MUL);
					advance();
					break;
				}
				case '/':{
					if(peekNext() == '/'){
						while(!eof()){
							advance();
							if(is_nl(peek())){
								break;
							}
						}
					}else if(peekNext() == '*'){
						while(!eof()){
							advance();
							if(peek() == '*' && peekNext() == '/'){
								break;
							}
						}
						advance(); // Skip `*`
						advance(); // Skip `/`
					}else{
						add_token(OP_DIV);
						advance();
					}
					break;
				}
				case '%':{
					add_token(OP_MOD);
					advance();
					break;
				}
				case ';':{
					add_token(OP_SEMICOLON);
					advance();
					break;
				}
				case '(':{
					add_token(OP_LPAREN);
					advance();
					break;
				}
				case ')':{
					add_token(OP_RPAREN);
					advance();
					break;
				}
				case '{':{
					add_token(OP_LBRACE);
					advance();
					break;
				}
				case '}':{
					add_token(OP_RBRACE);
					advance();
					break;
				}
				case ',':{
					add_token(OP_COMMA);
					advance();
					break;
				}
				case ':':{
					add_token(OP_COLON);
					advance();
					break;
				}
				case '.':{
					if(is_digit(peekNext())){
						lex_number();
					}else{
						add_token(OP_DOT);
						advance();
					}
					break;
				}
				default:{
					unexpected_error();
				}
			}
		}
	}

	add_token(T_EOF);

	return tokens;
}

void Lexer::unexpected_error(){
	std::string error = "Unexpected token `"+ std::string(1, peek()) + "`";
	error += " at "+ std::to_string(line) +":"+ std::to_string(column);
	throw YoctoException(error);
}

void Lexer::unexpected_eof_error(){
	throw UnexpectedEofException();
}