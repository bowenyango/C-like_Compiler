#include "C0_Compiler.h"

mid_code* C0_Compiler::new_mid_code(mid_op op, string *var_1, string *var_2, string *des) {
	mid_code* new_one = new mid_code();
	new_one->op = op;
	new_one->var_1 = var_1;
	new_one->var_2 = var_2;
	new_one->des = des;

	return new_one;


}

mid_code* C0_Compiler::get_mid_code(int number) {
	return mid_code_table[number];
}

bool C0_Compiler::add_mid_code_table(mid_code* new_one) {
	if (mid_code_table_top < MAX_MID_CODE) {
		mid_code_table[mid_code_table_top] = new_one;
		mid_code_table_top++;
		return true;
	}
	else
		return false;
}

bool C0_Compiler::new_add_mid_code(mid_op op, string* var_1, string* var_2, string* des) {
	mid_code* temp_mid_code;
	temp_mid_code = new_mid_code(op, var_1, var_2, des);


	switch (op) {
	case CALLOP:
		midcodefile << "call " << *des << endl;
		break;
	case RETOP:
		midcodefile << "ret " << *des << endl;
		break;
	case ARRAYTOOP:
		midcodefile << *des << " = " << *var_1 << "[" << *var_2 << "]" << endl;
		break;
	case TOARRAYOP:
		midcodefile << *des << "[" << *var_2 << "]" << " = " << *var_1 << endl;
		break;
	case ADDOP:
		midcodefile << *des << " = " << *var_1 << " + " << *var_2 << endl;
		break;
	case SUBOP:
		midcodefile << *des << " = " << *var_1 << " - " << *var_2 << endl;
		break;
	case MULTOP:
		midcodefile << *des << " = " << *var_1 << " * " << *var_2 << endl;
		break;
	case DIVOP:
		midcodefile << *des << " = " << *var_1 << " / " << *var_2 << endl;
		break;
	case EQLEQLOP:
		midcodefile << "If " << *var_1 << " == " << *var_2 << endl;
		midcodefile << "    Then Goto " << *des << endl;
		break;
	case NEQOP:
		midcodefile << "If " << *var_1 << " != " << *var_2 << endl;
		midcodefile << "    Then Goto " << *des << endl;
		break;
	case GTROP:
		midcodefile << "If " << *var_1 << " > " << *var_2 << endl;
		midcodefile << "    Then Goto " << *des << endl;
		break;
	case GTREQLOP:
		midcodefile << "If " << *var_1 << " >= " << *var_2 << endl;
		midcodefile << "    Then Goto " << *des << endl;
		break;
	case LSSOP:
		midcodefile << "If " << *var_1 << " < " << *var_2 << endl;
		midcodefile << "    Then Goto " << *des << endl;
		break;
	case LSSEQLOP:
		midcodefile << "If " << *var_1 << " <= " << *var_2 << endl;
		midcodefile << "    Then Goto " << *des << endl;
		break;
	case LABELOP:
		midcodefile << *des << ":" << endl;
		break;
	case GOTOOP:
		midcodefile << "GOTO " << *des << endl;
		break;
	case BNZOP:
		midcodefile << "BNZ " << *var_1 << " " << *des << endl;
		break;
	case BZOP:
		midcodefile << "BZ " << *var_1 << " " << *des << endl;
		break;
	case FUNCBEGINOP:
		midcodefile << "init " << *des << endl;
		break;
	case SCANFOP:
		midcodefile << "scanf " << *var_2 << " " << *des << " " << endl; //*var_2是数据类型 *des是存储位置
		break;
	case PRINTFOP:
		midcodefile << "printf " << *var_2 << " " << *des << " " << endl; //*var_2是类型，*des是数据
		break;
	case EXITOP:
		midcodefile << "exit " << endl;
		break;
	case NOTOP:
		midcodefile << "nop " << endl;
		break;
	case PARAVALOP:
		midcodefile << "push " << *des << endl;
		break;
	case ASSIGNOP:
		midcodefile << *des << " = " << *var_1;
		break;
	}

	if (mid_code_table_top < MAX_MID_CODE) {
		mid_code_table[mid_code_table_top] = temp_mid_code;
		mid_code_table_top++;

		return true;
	}
	else
		return false;

//	return add_mid_code_table(temp_mid_code);
}

void C0_Compiler::push_string(string* string_val, int* string_index) {
	int i;
	string* temp;
	for (i = 0; i < string_number; i++) {
		temp = string_table[i];
		if (*temp == *string_val) {
			*string_index = i;
			return;
		}
	}
	*string_index = string_number;
	string_table[string_number] = string_val;
	string_number++;
}

void C0_Compiler::output_mid_code_tofile() {
	int i;
	mid_code* temp;
	string* var_1, *var_2, *des;

	for (i = 0; i < mid_code_table_top; i++) {
		temp = mid_code_table[i];
		var_1 = temp->var_1;
		var_2 = temp->var_2;
		des = temp->des;

		switch (temp->op) {
		case CALLOP:
			midcodefile << "call " << *des << endl;
			break;
		case RETOP:
			midcodefile << "ret " << *des << endl;
			break;
		case ARRAYTOOP:
			midcodefile << *des << " = " << *var_1 << "[" << *var_2 << "]" << endl;
			break;
		case TOARRAYOP:
			midcodefile << *des << "[" << *var_2 << "]" << " = " << *var_1 << endl;
			break;
		case ADDOP:
			midcodefile << *des << " = " << *var_1 << " + " << *var_2 << endl;
			break;
		case SUBOP:
			midcodefile << *des << " = " << *var_1 << " - " << *var_2 << endl;
			break;
		case MULTOP:
			midcodefile << *des << " = " << *var_1 << " * " << *var_2 << endl;
			break;
		case DIVOP:
			midcodefile << *des << " = " << *var_1 << " / " << *var_2 << endl;
			break;
		case EQLEQLOP:
			midcodefile << *des << " <= " << *var_1 << " == " << *var_2 << endl;
			break;
		case NEQOP:
			midcodefile << *des << " <= " << *var_1 << " != " << *var_2 << endl;
			break;
		case GTROP:
			midcodefile << *des << " <= " << *var_1 << " > " << *var_2 << endl;
			break;
		case GTREQLOP:
			midcodefile << *des << " <= " << *var_1 << " >= " << *var_2 << endl;
			break;
		case LSSOP:
			midcodefile << *des << " <= " << *var_1 << " < " << *var_2 << endl;
			break;
		case LSSEQLOP:
			midcodefile << *des << " <= " << *var_1 << " <= " << *var_2 << endl;
			break;
		case LABELOP:
			midcodefile << *des << ":" << endl;
			break;
		case GOTOOP:
			midcodefile << "GOTO " << *des << endl;
			break;
		case BNZOP:
			midcodefile << "BNZ " << *var_1 << " " << *des << endl;
			break;
		case BZOP:
			midcodefile << "BZ " << *var_1 << " " << *des << endl;
			break;
		case FUNCBEGINOP:
			midcodefile << "init " << *des << endl;
			break;
		case SCANFOP:
			midcodefile << "scanf " << *var_2 << " " << *des << " " << endl; //*var_2是数据类型 *des是存储位置
			break;
		case PRINTFOP:
			midcodefile << "printf " << *var_2 << " " << *des << " " << endl; //*var_2是类型，*des是数据
			break;
		case EXITOP:
			midcodefile << "exit " << endl;
			break;
		case NOTOP:
			midcodefile << "nop " << endl;
			break;
		case PARAVALOP:
			midcodefile << "push " << *des << endl;
			break;
		case ASSIGNOP:
			midcodefile << *des << " = " << *var_1;
		}
	}
}

string* C0_Compiler::new_temp() {
	string* temp = new string();
	symbol* temp_sym;
	string t;
	temp->append("$t");
	temp->append(to_string(temp_number));
	this->temp_number++;

	temp_sym = new_symbol(temp, -1, current_func, VARIDENT, INTRET);
	
	add_sym_table(temp_sym);

	return temp;
}

string* C0_Compiler::new_label() {
	string* temp = new string();
	temp->append("Label");
	temp->append(to_string(label_number));
	this->label_number++;

	return temp;
}

bool C0_Compiler::var_is_temp(string* temp_name) {

	if ((temp_name->length()) != 0 && (*temp_name).at(0) == '$')
		return true;
	else
		return false;
}

bool C0_Compiler::var_is_ret(string* ret_name) {
	
	if ((ret_name->length()) != 0 && (*ret_name).at(0) == '#') {
		
		return true;
	}
	else {
		
		return false;
	}
}

bool C0_Compiler::var_is_ident(string* ident_name) {
	
	if ((ident_name->length()) != 0 && ((*ident_name).at(0) == '_' || (*ident_name).at(0) >= 'a' && (*ident_name).at(0) <= 'z'))
		return true;
	else
		return false;
}

bool C0_Compiler::var_is_number(string* number_name) {

	if ((number_name->length()) != 0 && ((*number_name).at(0) >= '0' && (*number_name).at(0) <= '9' || (*number_name).at(0) == '-'))
		return true;
	else
		return false;
}

