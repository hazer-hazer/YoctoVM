#ifndef TOKEN_H
#define TOKEN_H

#include <variant>
#include <algorithm>
#include <vector>
#include <string>

enum TokenType {
    T_BOOL,
    T_INT,
    T_FLOAT,
    T_OP,
    T_KW,
    T_ID,
    T_NL,
    T_EOF
};

enum Operator {
    OP_ASSIGN,

    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD,

    // Punctuations
    OP_LPAREN, OP_RPAREN,
    OP_LBRACE, OP_RBRACE,

    OP_COMMA,

    OP_SEMICOLON
};

const std::vector <std::string> operators {
    "=",
    "+", "-", "*", "/", "%",

    "(", ")",
    "{", "}",

    ",",

    ";"
};

inline std::string op_to_str(const Operator & op){
    return operators.at(static_cast<int>(op));
}

enum Keyword {
    KW_IF, KW_ELIF, KW_ELSE,
    KW_VAR, KW_VAL,
    KW_TRUE, KW_FALSE,
    KW_FUNC,

    KW_MAX
};

const std::vector <std::string> keywords {
    "if", "elif", "else",
    "var", "val",
    "true", "false",
    "func"
};

inline Keyword str_to_kw(const std::string & str){
    return static_cast<Keyword>(
        std::distance(keywords.begin(), std::find(keywords.begin(), keywords.end(), str))
    );
}

inline std::string kw_to_str(const Keyword & kw){
    return keywords.at(static_cast<int>(kw));
}

// Note: In Yocto floating-point numbers are doubles

typedef std::variant<bool, int, double, std::string, Operator, Keyword> TokenVal;

typedef struct {
    uint32_t line = 0;
    uint32_t column = 0;
    uint32_t startIndex = 0;
    uint32_t endIndex = 0;
} Position;

struct Token {
    TokenType type;
    TokenVal val;

    Position pos;

    Token(const TokenType & _type, const std::string & v){
        this->type = _type;

        switch(type){
            case T_INT:{
                val = std::stoi(v);
                break;
            }
            case T_FLOAT:{
                val = std::stod(v);
                break;
            }
            case T_ID:{
                val = v;
                break;
            }
            case T_NL:{
                val = 0;
                break;
            }
            case T_EOF:{
                val = 0;
                break;
            }
        }
    }

    Token(const Operator & op){
        type = T_OP;
        val = op;
    }

    Token(const Keyword & kw){
        if(kw == KW_TRUE || kw == KW_FALSE){
            type = T_BOOL;
            val = kw == KW_TRUE;
        }else{
            type = T_KW;
            val = kw;
        }
    }

    Token(const TokenType & type){
        this->type = type;
        val = 0;
    }

    virtual ~Token() = default;

    bool Bool(){
        return std::get<bool>(val);
    }

    int Int(){
        return std::get<int>(val);
    }

    double Float(){
        return std::get<double>(val);
    }

    std::string String(){
        return std::get<std::string>(val);
    }

    Operator op(){
        return std::get<Operator>(val);
    }
    
    Keyword kw(){
        return std::get<Keyword>(val);
    }

    std::string to_string(){
        std::string str;

        switch(type){
            case T_BOOL:{
                str += "bool";
                break;
            }
            case T_INT:{
                str += "int";
                break;
            }
            case T_FLOAT:{
                str += "float";
                break;
            }
            case T_ID:{
                str += "identifier";
                break;
            }
            case T_OP:{
                str += "operator";
                break;
            }
            case T_KW:{
                str += "keyword";
                break;
            }
            case T_NL:{
                str += "[new line]";
                break;
            }
            case T_EOF:{
                str += "[EOF]";
                break;
            }
        }

        // TODO: Fix quote for empty values
        str += " `";
        switch(type){
            case T_BOOL:{
                str += std::to_string(Bool());
                break;
            }
            case T_INT:{
                str += std::to_string(Int());
                break;
            }
            case T_FLOAT:{
                str += std::to_string(Float());
                break;
            }
            case T_ID:{
                str += String();
                break;
            }
            case T_OP:{
                str += operators.at(op());
                break;
            }
            case T_KW:{
                str += keywords.at(kw());
                break;
            }
        }

        str += "` at "+ std::to_string(pos.line) +":"+ std::to_string(pos.column);

        return str;
    }

    void error(const std::string & msg){
        throw msg + " " + to_string();
    }
};

typedef std::vector<Token> TokenStream;

#endif