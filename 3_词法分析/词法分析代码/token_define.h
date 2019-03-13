#ifndef TOKEN_DEFINE_H_INCLUDED
#define TOKEN_DEFINE_H_INCLUDED

#include <iostream>

//TOEKN_TYPE表示token是什么，后面+SYMBOL说明是保留字
//IDENT_TYPE表示标识符的种类，表示的是变量、常量还是函数
//IDENT_OBJC表示标识符的类型，是int还是char还是数组还是字符数组或者void

#define TOKEN_LENGTH 20
#define TOKEN_LIMIT 10000

enum token_type{
    PLUS,           //+
    SUB,            //-
    MULT,           //*
    DIV,            // /
    BECOME,         // =
    EQLEQL,         // ==
    NEQ,            // !=
    GTR,            // >
    GTREQL,         // >=
    LSS,            // <
    LSSEQL,         // <=
    INT,            // 数字
    CHAR,           // 单个字符 'x'
    STRING,         // 字符串 ""
    CONST_SYMBOL,   // const
    INT_SYMBOL,     // int
    CHAR_SYMBOL,    // char
    VOID_SYMBOL,    // void
    MAIN_SYMBOL,    // main
    IF_SYMBOL,      // if
    ELSE_SYMBOL,    // else
    SWITCH_SYMBOL,  // switch
    CASE_SYMBOL,    // case
    DEFAULT_SYMBOL, // default
    SCANF_SYMBOL,   // scanf
    PRINTF_SYMBOL,  // printf
    RETURN_SYMBOL,  // return
    DO_SYMBOL,      // do
    WHILE_SYMBOL,   // while
    LEFT_PAR,       // (
    RIGHT_PAR,      // )
    LEFT_BRACKET,   // [
    RIGHT_BRACKET,  // ]
    LEFT_BRACE,     // {
    RIGHT_BRACE,    // }
    COMMA,          // ,
    COLON,          // :
    SEMICOLON,      // ;

    IDENT           // 标识符

    //引号不再单独出现在token内，而是融入char和string
};

static char token_type_enum_to_string[][TOKEN_LENGTH] = {
    "PLUS",           //+
    "SUB",            //-
    "MULT",           //*
    "DIV",            // /
    "BECOME",         // =
    "EQLEQL",         // ==
    "NEQ",            // !=
    "GTR",            // >
    "GTREQL",         // >=
    "LSS",            // <
    "LSSEQL",         // <=
    "INT",            // 数字
    "CHAR",           // 单个字符 'x'
    "STRING",         // 字符串 ""
    "CONST_SYMBOL",   // const
    "INT_SYMBOL",     // int
    "CHAR_SYMBOL",    // char
    "VOID_SYMBOL",    // void
    "MAIN_SYMBOL",    // main
    "IF_SYMBOL",      // if
    "ELSE_SYMBOL",    // else
    "SWITCH_SYMBOL",  // switch
    "CASE_SYMBOL",    // case
    "DEFAULT_SYMBOL", // default
    "SCANF_SYMBOL",   // scanf
    "PRINTF_SYMBOL",  // printf
    "RETURN_SYMBOL",  // return
    "DO_SYMBOL",      // do
    "WHILE_SYMBOL",   // while
    "LEFT_PAR",       // (
    "RIGHT_PAR",      // )
    "LEFT_BRACKET",   // [
    "RIGHT_BRACKET",  // ]
    "LEFT_BRACE",     // {
    "RIGHT_BRACE",    // }
    "COMMA",          // ,
    "COLON",          // :
    "SEMICOLON",      // ;

    "IDENT"           // 标识符
};

static char token_symbol[][TOKEN_LENGTH]{
    "const",
    "int",
    "char",
    "void",
    "main",
    "if",
    "else",
    "switch",
    "case",
    "default",
    "scanf",
    "printf",
    "return",
    "do",
    "while"
};

union token_value{
    int number;
    char letter;
    std::string *pstring;
};

enum token_value_type{
    NUMBER,
    LETTER,
    PSTRING
};

typedef struct{
    int line;
    int pos;
    token_type t_type;
    token_value t_value;
    token_value_type t_value_type;
}token;

static token *token_table[TOKEN_LIMIT];

#endif // TOKEN_DEFINE_H_INCLUDED
