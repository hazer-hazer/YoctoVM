#ifndef TOKEN_H
#define TOKEN_H

#include <variant>
#include <algorithm>
#include <vector>
#include <string>

// Note: In Yocto floating-point numbers are doubles
enum class Operator;
enum class Keyword;
using TokenVal = std::variant<bool, int, double, std::string, Operator, Keyword>;

struct Token;
using TokenStream = std::vector<Token>;

enum class TokenType {
    T_NULL,
    T_BOOL,
    T_INT,
    T_FLOAT,
    T_STR,
    T_OP,
    T_KW,
    T_ID,
    T_NL,
    T_EOF
};

enum class Operator {
    ASSIGN,

    ADD, SUB, MUL, DIV, MOD,

    // Punctuations
    LPAREN, RPAREN,
    LBRACE, RBRACE,

    COMMA, COLON, DOT,

    SEMICOLON
};

const std::vector <std::string> operators {
    "=",
    "+", "-", "*", "/", "%",

    "(", ")",
    "{", "}",

    ",", ":", ".",

    ";"
};

inline std::string op_to_str(const Operator & op){
    return operators.at(static_cast<int>(op));
}

enum class Keyword {
    KW_NULL,
    KW_TRUE, KW_FALSE,
    KW_VAR, KW_VAL,
    KW_FUNC,

    KW_IF, KW_ELIF, KW_ELSE,
    
    KW_MAX
};

// Overload operator less-than to check if identifier is Keyword
inline bool operator<(Keyword kwl, Keyword kwr){
    return static_cast<int>(kwl) < static_cast<int>(kwr);
}

const std::vector <std::string> keywords {
    "null",
    "true", "false",
    "var", "val",
    "func",
    "if", "elif", "else"
};

inline Keyword str_to_kw(const std::string & str){
    return static_cast<Keyword>(
        std::distance(keywords.begin(), std::find(keywords.begin(), keywords.end(), str))
    );
}

inline std::string kw_to_str(const Keyword & kw){
    return keywords.at(static_cast<int>(kw));
}

struct Position {
    uint32_t line = 0;
    uint32_t column = 0;
};

struct Token {
    TokenType type;
    TokenVal val;

    Position pos;

    Token(const TokenType & _type, const std::string & v){
        this->type = _type;

        switch(type){
            case TokenType::T_ID:
            case TokenType::T_STR:{
                val = v;
                break;
            }
            case TokenType::T_NL:{
                val = 0;
                break;
            }
            case TokenType::T_EOF:{
                val = 0;
                break;
            }
        }
    }

    Token(const int & i){
        type = TokenType::T_INT;
        val = i;
    }

    Token(const double & d){
        type = TokenType::T_FLOAT;
        val = d;
    }

    Token(const Operator & op){
        type = TokenType::T_OP;
        val = op;
    }

    Token(const Keyword & kw){
        switch(kw){
            case Keyword::KW_TRUE:
            case Keyword::KW_FALSE:{
                type = TokenType::T_BOOL;
                val = kw == Keyword::KW_TRUE;
                break;
            }
            case Keyword::KW_NULL:{
                type = TokenType::T_NULL;
                val = 0;
            }
            default:{
                type = TokenType::T_KW;
                val = kw;
            }
        }
    }

    Token(const TokenType & type){
        this->type = type;
        val = false;
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
            case TokenType::T_BOOL:{
                str += "bool";
                break;
            }
            case TokenType::T_INT:{
                str += "int";
                break;
            }
            case TokenType::T_FLOAT:{
                str += "float";
                break;
            }
            case TokenType::T_ID:{
                str += "identifier";
                break;
            }
            case TokenType::T_STR:{
                str += "string";
                break;
            }
            case TokenType::T_OP:{
                str += "operator";
                break;
            }
            case TokenType::T_KW:{
                str += "keyword";
                break;
            }
            case TokenType::T_NL:{
                str += "[new line]";
                break;
            }
            case TokenType::T_EOF:{
                str += "[EOF]";
                break;
            }
        }

        // TODO: Fix quote for empty values
        str += " `";
        switch(type){
            case TokenType::T_BOOL:{
                str += std::to_string(Bool());
                break;
            }
            case TokenType::T_INT:{
                str += std::to_string(Int());
                break;
            }
            case TokenType::T_FLOAT:{
                str += std::to_string(Float());
                break;
            }
            case TokenType::T_ID:
            case TokenType::T_STR:{
                str += String();
                break;
            }
            case TokenType::T_OP:{
                str += op_to_str(op());
                break;
            }
            case TokenType::T_KW:{
                str += kw_to_str(kw());
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

#endif