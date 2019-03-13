#ifndef C0_COMPILER_H_INCLUDED
#define C0_COMPILER_H_INCLUDED

#include <iostream>
#include <fstream>
#include "token_define.h"
#include "error_define.h"
#include <string.h>
#include <algorithm>
using namespace std;


class C0_Compiler{

public:
    C0_Compiler();
    void words_analysis();

    //Used For Token Analysis
    string current_token;
    string new_line;
    char current_char;
    int current_line;
    int begin_pos;
    int current_pos;
    ifstream infile;
    ofstream outfile;
    ofstream errorfile;
    token* temp;
    int token_number;

    //Used For Read File
    void clear_token();
    void next_letter();
    void back_letter();
    void next_line();

    //current_letter
    bool is_plus();
    bool is_sub();
    bool is_mult();
    bool is_div();
    bool is_become();
    bool is_exm();
    bool is_gtr();
    bool is_lss();
    bool is_int();
    bool is_char();
    bool is_instring();

    bool is_double_quot();
    bool is_single_quot();

    bool is_left_par();
    bool is_right_par();
    bool is_left_bracket();
    bool is_right_bracket();
    bool is_left_brace();
    bool is_right_brace();
    bool is_comma();
    bool is_colon();
    bool is_semicolon();

    bool is_space();
    bool is_tab();
    bool is_null();
    void skip_spacetab();

    //current_token
    int is_symbol();

    //
    void addin_table();
    void ERROR(int err_typ);

    void print_token_table();
};




#endif // C0_COMPILER_H_INCLUDED
