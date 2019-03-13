#include "C0_Compiler.h"
std::hash<std::string> str_hash;

int C0_Compiler::name_hash(string* name) {
	return str_hash(*name) % MAX_HASH_NUMBER;
}

symbol* C0_Compiler::find_symbol(string*name, int which_func, bool is_local) {
	symbol* temp;
	int hash_index = name_hash(name);
	int sym_index = hash_table[hash_index];
	int func_begin_index = func_begin[which_func];

	if (is_local) { //局部查找
		while (sym_index >= func_begin_index) {
			temp = sym_table[sym_index];

			if (*temp->name == *name) {
				return temp;
			}
			else
				sym_index = temp->link;
		}
	}
	else {
		while (sym_index >= 0) {
			temp = sym_table[sym_index];
			if (*temp->name == *name)
				return temp;
			else
				sym_index = temp->link;
		}
	}
	return nullptr;
}

bool C0_Compiler::add_sym_table(symbol* new_symbol) {
	if (sym_table_top < MAX_SYMBOL_NUMBER) {
		int hash_index = name_hash(new_symbol->name);
		new_symbol->link = hash_table[hash_index];
		hash_table[hash_index] = sym_table_top;
		sym_table[sym_table_top] = new_symbol;
		sym_table_top++;

		cout << sym_table_top << " : " << *new_symbol->name << endl;
		return true;
	}
	else
		return false;
}

bool C0_Compiler::sym_exist(string *name, int which_func, bool is_local) {
	symbol* temp;
	temp = find_symbol(name, which_func, is_local);
	if (temp != nullptr)
		return true;
	else
		return false;
}
bool C0_Compiler::sym_is_func(string* name, int which_func, bool is_local) {
	symbol* temp;
	temp = find_symbol(name, which_func, is_local);
	if (temp == nullptr) {
		cout << "ERROR! sym is not exist in FUNCTION sym_is_func." << endl;
		return false;
	}
	else {
		if (temp->i_type == FUNCIDENT)
			return true;
		else
			return false;
	}
}

bool C0_Compiler::sym_is_var(string* name, int which_func, bool is_local) {
	symbol* temp;
	temp = find_symbol(name, which_func, is_local);
	if (temp == nullptr) {
		cout << "ERROR! sym is not exist in FUNCTION sym_is_var." << endl;
		return false;
	}
	else {
		if (temp->i_type == VARIDENT)
			return true;
		else
			return false;
	}
}

bool C0_Compiler::sym_is_const(string* name, int which_func, bool is_local) {
	symbol* temp;
	temp = find_symbol(name, which_func, is_local);
	if (temp == nullptr) {
		cout << "ERROR! sym is not exist in FUNCTION sym_is_const." << endl;
		return false;
	}
	else {
		if (temp->i_type == CONSTIDENT)
			return true;
		else
			return false;
	}
}

bool C0_Compiler::sym_is_array(string* name, int which_func, bool is_local) {
	symbol* temp;
	temp = find_symbol(name, which_func, is_local);
	if (temp == nullptr) {
		cout << "ERROR! sym is not exist in FUNCTION sym_is_array." << endl;
		return false;
	}
	else {
		if (temp->i_type == ARRAYIDENT)
			return true;
		else
			return false;
	}
}

symbol* C0_Compiler::new_symbol(string* name, int number, int which_func, ident_type i_type, ret_type r_type) {
	symbol* temp_symbol = new symbol();

	//int hash_index = name_hash(name);
	//temp_symbol->link = hash_table[hash_index];
	//hash_table[hash_index] = sym_table_top;
	//sym_table[sym_table_top] = temp_symbol;
	//sym_table_top++;


	temp_symbol->name = name;
	temp_symbol->address = store_address;
	temp_symbol->number = number;
	temp_symbol->which_func = which_func;
	temp_symbol->i_type = i_type;
	temp_symbol->r_type = r_type;

	switch (temp_symbol->i_type) {
	case CONSTIDENT:
	case VARIDENT:
	case PARAIDENT:
		store_address++;
		break;
	case ARRAYIDENT:
		store_address += temp_symbol->number;
		break;
	case FUNCIDENT:
		break;
	}

	return temp_symbol;
}

void C0_Compiler::main_sym_table_init() {
	current_func = 0;
	func_begin[current_func] = 0;
}

void C0_Compiler::sym_table_init() {
	int i;
	sym_table_top = 0;
	for (i = 0; i < MAX_FUNC_NUMBER; i++)
		func_begin[i] = -1;
	for (i = 0; i < MAX_HASH_NUMBER; i++)
		hash_table[i] = -1;
	for (i = 0; i < MAX_SYMBOL_NUMBER; i++)
		sym_table[i] = nullptr;
}

void C0_Compiler::push_func() {
	func_number++;

	current_func = func_number;
	func_begin[current_func] = sym_table_top;

	func_address = store_address; //全局函数名记录
	store_address = 0;//函数内从0开始
}

void C0_Compiler::pop_func() {
	int sym_index = func_begin[current_func];
	symbol * temp;
	int hash;

	//函数内标识符的个数
	func_sym_number[current_func] = sym_table_top - sym_index;
	//函数所需空间
	func_space[current_func] = store_address;
	
	//用于存储函数内的标识符
	func_sym_table[current_func] = new symbol*[300];

	//把总符号栈中的当前函数内标识符出栈，并存到func_sym_table中
	while (sym_table_top > sym_index) {
		sym_table_top--;
		temp = sym_table[sym_table_top];
		hash = name_hash(temp->name);
		hash_table[hash] = temp->link;

		func_sym_table[current_func][sym_table_top - sym_index] = temp;
	}
	//函数名在main中的标识符
	temp = sym_table[sym_table_top - 1];
	temp->func_ref = current_func;

	store_address = func_address; //继续从全局函数名开始

	current_func = 0; //回到最外层
}

void C0_Compiler::output_symtab_tofile(int current_line) {
	if (C0_Compiler::current_line == current_line) {
		int i;
		symbol *temp;
		for (i = 0; i < sym_table_top; i++) {
			temp = sym_table[i];
			current_symtab_file << i << "\t" << *temp->name << "\t" << temp->address << "\t" << temp->number << "\t"
				<< temp->which_func << "\t" << ident_type_enum_to_string[temp->i_type] << "\t" 
				<< ret_type_enum_to_string[temp->r_type] << "\t" << temp->link << endl;
		}
	}
}