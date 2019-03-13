#ifndef C0_COMPILER_H_INCLUDED
#define C0_COMPILER_H_INCLUDED

#include <iostream>
#include <fstream>
#include <functional>
#include "token_define.h"
#include "error_define.h"
#include "symbol_table.h"
#include "mid_code.h"
#include "object_code.h"
#include <string.h>
#include <string>
#include <algorithm>
#include <sstream>
using namespace std;


class C0_Compiler {

public:
	C0_Compiler();
	void words_analysis();

	//*************************************
	//Used For Token Analysis
	string current_symbol_string;
	string new_line;
	char current_char;
	int current_line;
	int begin_pos;
	int current_pos;
	ifstream infile;
	ofstream outfile;
	ofstream current_symtab_file;
	ofstream errorfile;
	ofstream midcodefile;
	ofstream objectcodefile;
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
	ret_type return_state_list[200];
	int return_state_number;

	void signed_int(int *para_length_number);
	ret_type type_ident();
	string* ident();
	void program();
	void const_decl();
	void const_def();
	void declare_head();
	void var_decl();
	void var_def();
	void ret_func_def(string* func_name);
	void non_ret_func_def(string* func_name);
	void complex_state(string* func_name);
	void para_list(symbol *func_symbol);
	void main_def(string* func_name);
	ret_type expression(string* var_name);
	ret_type term(string* var_name);
	ret_type factor(string* var_name);
	void statement(string* func_name);
	void become_state();
	void if_state(string* func_name);
	void condition(string* label_name);
	void dowhile_state(string* func_name);
	int const_value();
	void switch_state(string* func_name);
	void case_list(string* done_label, string* func_name, string* switch_name);
	int case_state(string* next_label, string* done_label, string* func_name, string* switch_name);
	void default_state(string* func_name, string* switch_name);
	void ret_func_call(symbol* func_sym);
	void non_ret_func_call(symbol* func_sym);
	void val_para_list(symbol* func_sym);
	void statement_list(string* func_name);
	void scanf_state();
	void printf_state();
	void ret_state(string* func_name);

	void grammar_analysis();

	//Symbol Table
	int sym_table_top = 0;
	int store_address = 0;

	string *symbol_name;
	int symbol_para_length_number;
	int current_func;
	int func_number;
	int func_address = 0;
	ident_type symbol_i_type;
	ret_type symbol_r_type;
	symbol* current_symbol;

	symbol* find_symbol(string* name, int which_func, bool is_local);
	int name_hash(string* name);
	symbol* new_symbol(string* name, int number, int which_func, ident_type i_type, ret_type r_type);
	bool add_sym_table(symbol* new_symbol);

	bool sym_exist(string* name, int which_func, bool is_local);
	bool sym_is_func(string* name, int which_func, bool is_local);
	bool sym_is_var(string* name, int which_func, bool is_local);
	bool sym_is_array(string* name, int which_func, bool is_local);
	bool sym_is_const(string* name, int which_func, bool is_local);


	void push_func();
	void pop_func();
	void main_sym_table_init();
	void sym_table_init();
	void output_symtab_tofile(int current_line);

	//Middle Code..
	mid_op op;
	string *var_1, *var_2, des;

	int mid_code_table_top;
	int temp_number;
	int label_number;
	int string_number;

	mid_code* new_mid_code(mid_op op, string* var_1, string* var_2, string* des);
	bool add_mid_code_table(mid_code *new_one);
	bool new_add_mid_code(mid_op op, string* var_1, string* var_2, string* des);
	mid_code* get_mid_code(int number);

	void output_mid_code_tofile();
	string* new_temp();
	string* new_label();
	bool var_is_temp(string* temp_name);
	bool var_is_ret(string* ret_name);
	bool var_is_ident(string* ident_name);
	bool var_is_number(string* number_name);
	void push_string(string* string_val, int* string_index);

	//Object Code..

	std::string *mipsCodes[10000];
	int mipsIndex;
	void object_init();
	void reg_init();
	void init_ascii();
	void generate();
	void generate_mips(std::string *order, std::string *rd, std::string *rs, std::string *rt);
	void generate_mips(std::string *order, std::string *rs, int baseaddress, std::string *rt);
	void generate_mips(std::string *order, std::string *rs, int num);
	void generate_mips(std::string *order, std::string *rd, std::string *rt, int imme);
	void generate_mips(std::string *order, std::string *rs, std::string *label);
	void generate_mips(std::string *order, std::string *target);
	void generate_mips(std::string *order);
	void push_mips(std::string *order);
	void write_mips_to_file();

	void func_begin(std::string *name);
	int currentRef;
	void gen_mips_label(std::string *label);
	void find_symbol(std::string *name, symbol **resultsym, bool *global);
	void get_use_reg(std::string *rs, std::string *reg);
	void get_result_reg(std::string *rd, std::string *reg);
	void writeBack(std::string *rd, std::string *reg);
	void handle_branch(mid_code *code);
	void handle_goto(std::string *label);
	void handle_para_val(std::string *para);
	void handle_arrayto(mid_code *code);
	void handle_call(std::string *name);
	void handle_ret(std::string *name);
	void handle_add(mid_code *code);
	void handle_sub(mid_code *code);
	void handle_mult_div(mid_code *code);

	void handle_scanf(mid_code *code);
	void handle_printf(mid_code *code);
	void handle_exit();
	void handle_toarray(mid_code *code);


};




#endif // C0_COMPILER_H_INCLUDED
#pragma once
