#ifndef C0_COMPILER_H_INCLUDED
#define C0_COMPILER_H_INCLUDED

#include <iostream>
#include <fstream>
#include <functional>
#include "token_define.h"
#include "error_define.h"
#include "symbol_table.h"
#include <string.h>
#include <algorithm>
using namespace std;


class C0_Compiler {

public:
	C0_Compiler();
	void words_analysis();

	//*************************************
	//Used For Token Analysis
	string current_symbol;
	string new_line;
	char current_char;
	int current_line;
	int begin_pos;
	int current_pos;
	ifstream infile;
	ofstream outfile;
	ofstream errorfile;
	token* temp;
	token* current_token;
	int token_number;

	streampos last_spot;
	char last_current_char;
	int last_current_line;
	int last_begin_pos;
	int last_current_pos;
	string last_line;
	string last_symbol;
	token* last_token;
	int last_token_number;

	//Used For Read File
	void clear_token();
	void next_letter();
	void back_letter();
	void next_line();

	void next_symbol();
	void save_spot();
	void back_spot();

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
	bool is_nextline();
	bool is_null();
	void skip_spacetab();

	//current_symbol
	int is_symbol();

	//
	void addin_table();
	void ERROR(int err_typ);

	void print_token_table();

	//*************************************
	//Used By Grammar Analysis
	void signed_int();
	void type_ident();
	void ident();
	void program();
	void const_decl();
	void const_def();
	void declare_head();
	void var_decl();
	void var_def();
	void ret_func_def();
	void non_ret_func_def();
	void complex_state();
	void para_list();
	void main_def();
	void expression();
	void term();
	void factor();
	void statement();
	void become_state();
	void if_state();
	void condition();
	void dowhile_state();
	void const_value();
	void switch_state();
	void case_list();
	void case_state();
	void default_state();
	void ret_func_call();
	void non_ret_func_call();
	void val_para_list();
	void statement_list();
	void scanf_state();
	void printf_state();
	void ret_state();

	void grammar_analysis();

	//Symbol Table
	int sym_table_top = 0;
	int store_address = 0;

	symbol* find_symbol(string* name, int which_func, bool is_local);
	int name_hash(string* name);
	symbol* new_symbol(string* name, int number, int level, ident_type i_type, ret_type r_type);
	bool add_sym_table(symbol* new_symbol);

	bool sym_exist(string* name, int which_func, bool is_local);
	bool sym_is_func(string* name, int which_func, bool is_local);
	bool sym_is_var(string* name, int which_func, bool is_local);
	bool sym_is_array(string* name, int which_func, bool is_local);
	bool sym_is_const(string* name, int which_func, bool is_local);


	void push_func();
	void pop_func();




};




#endif // C0_COMPILER_H_INCLUDED
#pragma once
