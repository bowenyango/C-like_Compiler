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
#include "optimize_code.h"
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

	bool replace(std::string& str, const std::string& from, const std::string& to);
	void replaceAll(std::string& str, const std::string& from, const std::string& to);

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
	int func_number = -1; //函数符号表的第一个为0
	int func_address = 0;
	ident_type symbol_i_type;
	ret_type symbol_r_type;
	symbol* current_symbol;

	symbol *sym_table[MAX_SYMBOL_NUMBER];
	int hash_table[MAX_HASH_NUMBER];
	int func_begin[MAX_FUNC_NUMBER];
	symbol **func_sym_table[MAX_FUNC_NUMBER];
	int func_space[MAX_FUNC_NUMBER];
	int func_sym_number[MAX_FUNC_NUMBER];

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
	mid_code* mid_code_table[MAX_MID_CODE];

	mid_code* new_mid_code(mid_op op, string* var_1, string* var_2, string* des);
	bool add_mid_code_table(mid_code *new_one);
	bool new_add_mid_code(mid_op op, string* var_1, string* var_2, string* des);
	mid_code* get_mid_code(int number);

	void output_mid_code_tofile(string path);
	string* new_temp();
	string* new_label();
	bool var_is_temp(string* temp_name);
	bool var_is_ret(string* ret_name);
	bool var_is_ident(string* ident_name);
	bool var_is_number(string* number_name);
	void push_string(string* string_val, int* string_index);

	//Optimize Code..
	bool block_begin_flag[MAX_MID_CODE]; //这样避免重复计算，使用bool
	int block_begin[MAX_BASIC_BLOCK];
	int block_number;
	fstream block_file;
	fstream data_stream_file;
	fstream dag_file;
	Block* block_table[MAX_BASIC_BLOCK];
	int func_block_begin[MAX_FUNC_NUMBER];
	bool **out_set;
	bool **use;
	bool **def;
	bool **in;
	bool **out;

	void divide_block();
	void use_def_init();
	void init_block_connect();
	void init_optimize();
	int find_label(string* label);
	
	void data_flow();
	int get_index(int func_ref, string* name);


	//Object Code..
	int mips_code_table_top;
	int current_mips_func = -1;
	string* string_table[MAX_MID_CODE];
	string* all_reg[MAX_REG_AVAILABLE];
	string* mips_code_table[MAX_MIPS_NUMBER];

	void mips_init();
	void reg_init();
	void ascii_init();
	void new_mips(string* op_code, string* rd, string* rs, string* rt); // R add
	void new_mips(string* op_code, string* rs, int offset, string* rt); // lw sw
	void new_mips(string* op_code, string* rs, int number); //
	void new_mips(string* op_code, string* rd, string* rt, int immediate); //I addi
	void new_mips(string* op_code, string* rs, string* label); //R jal
	void new_mips(string* op_code, string* target); //?
	void new_mips(string* op_code); //syscall

	void add_mips_table(string* mips);
	void output_mips_code(string path);
	
	void new_mips_label(string* label);
	symbol* find_symbol(string* name, bool* is_local);
	void get_use_reg(string* rs, string* reg); // load word ?
	void get_result_reg(string* rd, string* reg); // store word ?
	void write_back(string* rd, string* reg); // ?

	void mips_func_init(string* func_name); //?
	void mips_branch(mid_code* temp_midcode);
	void mips_goto(string* label);
	void mips_paraval(string* paraval);
	void mips_arrayto(mid_code* temp_midcode);
	void mips_toarray(mid_code* temp_midcode);
	void mips_call(string* func_name);
	void mips_ret(string* ret_name);
	void mips_add(mid_code* temp_midcode);
	void mips_sub(mid_code* temp_midcode);
	void mips_mult(mid_code* temp_midcode);
	void mips_div(mid_code* temp_midcode);
	void mips_scanf(mid_code* temp_midcode);
	void mips_printf(mid_code* temp_midcode);
	void mips_exit(mid_code* temp_mid_code);
	void mid_to_mips();
	void mips_generate(string path);

};




#endif // C0_COMPILER_H_INCLUDED
#pragma once
