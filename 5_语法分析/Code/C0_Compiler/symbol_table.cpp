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

	if (is_local) { //¾Ö²¿²éÕÒ
		while (sym_index >= func_begin_index) {
			temp = sym_table[sym_index];
			if (*temp->name == *name)
				return temp;
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
		sym_table[sym_table_top] = new_symbol;
		sym_table_top++;
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

	int hash_index = name_hash(name);
	temp_symbol->link = hash_index;
	hash_table[hash_index] = sym_table_top;
	sym_table[sym_table_top] = temp_symbol;
	sym_table_top++;


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
		;
	}

	return temp_symbol;
}

