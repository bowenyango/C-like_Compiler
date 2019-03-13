#include "C0_Compiler.h"

void C0_Compiler::mips_init() {
	mips_code_table_top = 0;
	current_mips_func = -1;
	reg_init();
}

void C0_Compiler::reg_init() {
	int i;
	for (i = 0; i < MAX_REG_AVAILABLE; i++) {
		all_reg[i] = nullptr;
	}
}

void C0_Compiler::ascii_init() {
	new_mips(new string(".data"));
	int i;
	stringstream temp_sstring = stringstream();
	string* temp_string = new string();
	for (i = 0; i < string_number; i++) {

		temp_sstring << "$String" << i << ":.asciiz\"" << *string_table[i] << "\"" << endl;
	}
	*temp_string = temp_sstring.str();
	new_mips(temp_string);
}

void C0_Compiler::add_mips_table(string* mips) {
	mips_code_table[mips_code_table_top] = mips;
	mips_code_table_top++;
}

void C0_Compiler::output_mips_code() {
	int i;
	for (i = 0; i < mips_code_table_top; i++) {
		objectcodefile << *mips_code_table[i];
	}
}

void C0_Compiler::new_mips(string* op_code, string* rd, string* rs, string* rt) {// R add
	stringstream temp_sstring = stringstream();
	string* temp_string = new string();
	temp_sstring << *op_code << "\t" << *rd << ",\t" << *rs << ",\t" << *rt << endl;
	*temp_string = temp_sstring.str();
	add_mips_table(temp_string);

}

void C0_Compiler::new_mips(string* op_code, string* rs, int offset, string* rt) { // lw sw
	stringstream temp_sstring = stringstream();
	string* temp_string = new string();
	temp_sstring << *op_code << "\t" << *rs << ",\t" << offset << "(" << *rt << ")" << endl;
	*temp_string = temp_sstring.str();
	add_mips_table(temp_string);
}


void C0_Compiler::new_mips(string* op_code, string* rs, int number) {
	stringstream temp_sstring = stringstream();
	string* temp_string = new string();
	temp_sstring << *op_code << "\t" << *rs << ",\t" << number << endl;
	*temp_string = temp_sstring.str();
	add_mips_table(temp_string);
}
void C0_Compiler::new_mips(string* op_code, string* rd, string* rt, int immediate) { //I addi
	stringstream temp_sstring = stringstream();
	string* temp_string = new string();
	temp_sstring << *op_code << "\t" << *rd << ",\t" << *rt << ",\t" << immediate << endl;
	*temp_string = temp_sstring.str();
	add_mips_table(temp_string);
}
void C0_Compiler::new_mips(string* op_code, string* rs, string* label) { //R jal?
	stringstream temp_sstring = stringstream();
	string* temp_string = new string();
	temp_sstring << *op_code << "\t" << *rs << ",\t" << *label << endl;
	*temp_string = temp_sstring.str();
	add_mips_table(temp_string);
}

void C0_Compiler::new_mips(string* op_code, string* target) { //?
	stringstream temp_sstring = stringstream();
	string* temp_string = new string();
	temp_sstring << *op_code << "\t" << *target << endl;
	*temp_string = temp_sstring.str();
	add_mips_table(temp_string);
}
void C0_Compiler::new_mips(string* op_code) { //syscall
	stringstream temp_sstring = stringstream();
	string* temp_string = new string();
	temp_sstring << *op_code << endl;
	*temp_string = temp_sstring.str();
	add_mips_table(temp_string);
}

void C0_Compiler::new_mips_label(string* label) { //
	stringstream temp_sstring = stringstream();
	string* temp_string = new string();
	temp_sstring << *label << ":" << endl;
	*temp_string = temp_sstring.str();
	add_mips_table(temp_string);
}

symbol* C0_Compiler::find_symbol(string* name, bool *is_local) { //仅根据名字找，返回符号symbol信息和是否为全局变量
	symbol** current_func_sym_table = func_sym_table[current_mips_func];
	int current_func_sym_number = func_sym_number[current_mips_func];
	symbol* temp_symbol;
	int i;
	for (i = 0; i < current_func_sym_number; i++) {
		temp_symbol = current_func_sym_table[i];
		if (*temp_symbol->name == *name) {
			*is_local = true;
			return temp_symbol;
		}
	}
	for (i = 0; i < sym_table_top; i++) {
		temp_symbol = sym_table[i];
		if (*temp_symbol->name == *name) {
			*is_local = false;
			return temp_symbol;
		}
	}
}

void C0_Compiler::mips_func_init(string* func_name) { //
	symbol* temp_symbol = nullptr;
	//通过函数名找到符号表中的信息
	bool temp_bool = false;
	temp_symbol = find_symbol(func_name, &temp_bool); //??
	//生成函数入口
	new_mips_label(func_name);
	//空间分配，包括参数、常量、变量、所有寄存器、上次fp指针、上次ra指针
	int temp_func_space = func_space[temp_symbol->func_ref]; //当时存没存参数进去?
	int para_number = temp_symbol->number;

	if (para_number != 0) //参数
		new_mips(ADDI_STR, SP_STR, SP_STR, para_number * 4);

	for (int i = 0; i < MAX_REG_AVAILABLE; i++) { //存寄存器
		if (i <= 6) { //$t
			string* temp_string = new string("$t");
			temp_string->append(to_string(i));
			new_mips(SW_STR, temp_string, -4 * temp_func_space, SP_STR);
		}
		else { // $s
			string* temp_string = new string("$s");
			temp_string->append(to_string(i - 7));
			new_mips(SW_STR, temp_string, -4 * temp_func_space, SP_STR);
		}
		temp_func_space++;
	}

	new_mips(SW_STR, FP_STR, -4 * temp_func_space, SP_STR);
	temp_func_space++;
	new_mips(SW_STR, RA_STR, -4 * temp_func_space, SP_STR);
	temp_func_space++;

	new_mips(ADD_STR, FP_STR, R0_STR, SP_STR); // fp = sp;
	new_mips(ADDI_STR, SP_STR, SP_STR, -4 * temp_func_space); //sp = sp + func_space
}

void C0_Compiler::get_use_reg(string* rs, string* reg) {
	if (var_is_ret(rs)) {
		*reg = *V0_STR;
	}
	else if (var_is_number(rs)) {
		if (rs->at(0) == '0') {
			*reg = *R0_STR;
		}
		else {
			new_mips(LI_STR, reg, rs);
		}
	}
	else {
		bool temp_local = false;
		symbol* temp_symbol;
		temp_symbol = find_symbol(rs, &temp_local);

		int reg_index = temp_symbol->reg_index;
		if (reg_index == -1) {
			int offset = temp_symbol->address;
			cout << *temp_symbol->name << " : " << offset << endl;
			if (temp_local) {
				new_mips(LW_STR, reg, -offset * 4, FP_STR);
			}
			else {
				new_mips(LW_STR, reg, offset * 4, GP_STR);
			}
		}
		else {
			stringstream sstring = stringstream();
			if (reg_index <= 6) {
				sstring << "$t" << reg_index;
			}
			else {
				sstring << "$s" << reg_index;
			}
			*reg = sstring.str();
			if (temp_symbol->i_type == PARAIDENT &&
				(all_reg[reg_index] == nullptr || *all_reg[reg_index] != *temp_symbol->name)) {

				int offset = temp_symbol->address;
				new_mips(LW_STR, reg, -offset * 4, FP_STR);
			}
			all_reg[reg_index] = temp_symbol->name;
		}
	}
}

void C0_Compiler::get_result_reg(string* rd, string* reg) { //选择最合适的寄存器
	if (var_is_ret(rd)) { //如果是返回值的话
		*reg = *V0_STR;
	}
	else {
		bool temp_bool = false;
		symbol* temp_symbol = nullptr;
		temp_symbol = find_symbol(rd, &temp_bool);
		int reg_index = temp_symbol->reg_index;
		if (reg_index == -1) { //说明之前没有分配寄存器
			;
		}
		else {
			stringstream sstring = stringstream();

			if (reg_index <= 6) { // 0-6 $t
				sstring << "$t" << reg_index;
			}
			else { // 7-15 $s
				sstring << "$s" << reg_index - 7;
			}

			*reg = sstring.str();
			all_reg[reg_index] = temp_symbol->name;
		}
	}
}

void C0_Compiler::write_back(string* rd, string* reg) {
	if (!var_is_ret(rd)) {
		symbol* temp_symbol = nullptr;
		bool is_local = false;
		temp_symbol = find_symbol(rd, &is_local);
		if (temp_symbol != nullptr && temp_symbol->reg_index == -1) {
			int offset = temp_symbol->address;
			if (is_local) {
				new_mips(SW_STR, reg, -4 * offset, FP_STR);
			}
			else {
				new_mips(SW_STR, reg, 4 * offset, GP_STR);
			}
		}
	}
}
void C0_Compiler::mips_branch(mid_code* temp_midcode) {
	string* reg_1 = new string(*T7_STR);
	string* reg_2 = new string(*T8_STR);
	string* reg_3 = new string(*T9_STR);
	string* var_1 = temp_midcode->var_1;
	string* var_2 = temp_midcode->var_2;
	string* des = temp_midcode->des;
	mid_op temp_op = temp_midcode->op;

	if (temp_op == EQLEQLOP || temp_op == NEQOP) {
		if (var_is_number(var_1) && var_is_number(var_2)) {
			int val_1 = atoi(var_1->c_str());
			int val_2 = atoi(var_2->c_str());
			int val = val_1 - val_2;
			get_use_reg(&string(to_string(val)), reg_1);
			*reg_2 = *R0_STR;
		}
		else {
			get_use_reg(var_1, reg_1);
			get_use_reg(var_2, reg_2);
		}
		new_mips(temp_op == EQLEQLOP ? BEQ_STR : BNE_STR, reg_1, reg_2, des);
	}
	else {
		if (var_is_number(var_1)) {
			if (var_is_number(var_2)) {
				int val_1 = atoi(var_1->c_str());
				int val_2 = atoi(var_2->c_str());
				int val = val_1 - val_2;
				get_use_reg(&string(to_string(val)), reg_3);
			}
			else {
				get_use_reg(var_1, reg_1);
				get_use_reg(var_2, reg_2);
				new_mips(SUB_STR, reg_3, reg_1, reg_2);
			}
		}
		else {
			if (var_is_number(var_2)) {
				get_use_reg(var_1, reg_1);
				new_mips(SUBI_STR, reg_3, reg_1, var_2);
			}
			else {
				get_use_reg(var_1, reg_1);
				get_use_reg(var_2, reg_2);
				new_mips(SUB_STR, reg_3, reg_1, reg_2);
			}
		}
		switch (temp_op) {
		case GTREQLOP:
			new_mips(BGEZ_STR, reg_3, des);
			break;
		case GTROP:
			new_mips(BGTZ_STR, reg_3, des);
			break;
		case LSSEQLOP:
			new_mips(BLEZ_STR, reg_3, des);
			break;
		case LSSOP:
			new_mips(BLTZ_STR, reg_3, des);
			break;
		}
	}
}
void C0_Compiler::mips_goto(string* label) {
	new_mips(J_STR, label);
}
void C0_Compiler::mips_paraval(string* paraval) {
	string* reg_3 = new string(*T9_STR);
	get_use_reg(paraval, reg_3);
	new_mips(SW_STR, reg_3, 0, SP_STR);
	new_mips(SUBI_STR, SP_STR, SP_STR, 4);
}
void C0_Compiler::mips_arrayto(mid_code* temp_midcode) { // des = var_1[var_2]
	string* reg_1 = new string(*T7_STR);
	string* reg_2 = new string(*T8_STR);
	string* reg_3 = new string(*T9_STR);
	string* var_1 = temp_midcode->var_1;
	string* var_2 = temp_midcode->var_2;
	string* des = temp_midcode->des;

	bool is_local = false;
	symbol* temp_symbol;
	temp_symbol = find_symbol(var_1, &is_local);
	int offset = temp_symbol->address;

	get_use_reg(var_2, reg_2);
	get_result_reg(des, reg_3);
	new_mips(ADDI_STR, reg_1, reg_2, offset);
	new_mips(SLL_STR, reg_1, reg_1, 2);

	if (is_local) {
		new_mips(SUB_STR, reg_1, FP_STR, reg_1);
		new_mips(LW_STR, reg_3, 0, reg_1);
	}
	else {
		new_mips(ADDU_STR, reg_1, GP_STR, reg_1); // ?ADD or ADDU
		new_mips(LW_STR, reg_3, 0, reg_1);
	}
	write_back(des, reg_3);
}
void C0_Compiler::mips_toarray(mid_code* temp_midcode) { // des[var_2] = var_1
	string* reg_1 = new string(*T7_STR);
	string* reg_2 = new string(*T8_STR);
	string* reg_3 = new string(*T9_STR);
	string* var_1 = temp_midcode->var_1;
	string* var_2 = temp_midcode->var_2;
	string* des = temp_midcode->des;

	bool is_local = false;
	symbol* temp_symbol;
	temp_symbol = find_symbol(des, &is_local);
	int offset = temp_symbol->address;

	get_use_reg(var_1, reg_1);
	get_use_reg(var_2, reg_2);

	new_mips(ADDI_STR, reg_3, reg_2, offset);
	new_mips(SLL_STR, reg_3, reg_3, 2);

	if (is_local) {
		new_mips(SUB_STR, reg_3, FP_STR, reg_3);
		new_mips(SW_STR, reg_1, 0, reg_3);
	}
	else {
		new_mips(ADDU_STR, reg_3, GP_STR, reg_3); // ?ADD or ADDU
		new_mips(SW_STR, reg_1, 0, reg_3);
	}
}
void C0_Compiler::mips_call(string* func_name) {
	new_mips(JAL_STR, func_name);
}
void C0_Compiler::mips_ret(string* ret_name) { //入栈的逆序
	symbol* temp_symbol;
	bool temp_bool;
	temp_symbol = find_symbol(ret_name, &temp_bool);
	int temp_func_space = 1;
	new_mips(LW_STR, RA_STR, 4 * temp_func_space, SP_STR); // $ra
	temp_func_space++;
	new_mips(LW_STR, FP_STR, 4 * temp_func_space, SP_STR); // $fp
	temp_func_space++;

	reg_init(); //退出清空
	for (int i = MAX_REG_AVAILABLE - 1; i >= 0; i--) {
		string* temp_string;
		if (i <= 6) { // $t
			temp_string = new string("$t");
			temp_string->append(to_string(i));
		}
		else { // $s
			temp_string = new string("$s");
			temp_string->append(to_string(i - 7));
		}
		new_mips(LW_STR, temp_string, temp_func_space * 4, SP_STR);
		temp_func_space++;
	}

	temp_func_space += func_space[temp_symbol->func_ref] - 1; // para用不用-
	new_mips(ADDI_STR, SP_STR, SP_STR, temp_func_space * 4);
	new_mips(JR_STR, RA_STR);
}

void C0_Compiler::mips_add(mid_code* temp_midcode) {
	string* reg_1 = new string(*T7_STR);
	string* reg_2 = new string(*T8_STR);
	string* reg_3 = new string(*T9_STR);
	string* var_1 = temp_midcode->var_1;
	string* var_2 = temp_midcode->var_2;
	string* des = temp_midcode->des;
	//cout << *var_1 << " " << *var_2 << endl;
 	if (var_is_number(var_1)) {
		if (var_is_number(var_2)) {
			int val_1 = atoi(var_1->c_str());
			int val_2 = atoi(var_2->c_str());
			int val = val_1 + val_2;

			get_result_reg(des, reg_3);
			new_mips(LI_STR, reg_3, val);
		}
		else {
			get_use_reg(var_2, reg_2);
			get_result_reg(des, reg_3);
			new_mips(ADDI_STR, reg_3, reg_2, var_1);
		}
	}
	else {
		get_use_reg(var_1, reg_1);

		if (var_is_number(var_2)) {
			get_result_reg(des, reg_3);
			new_mips(ADDI_STR, reg_3, reg_1, var_2);
		}
		else {
			get_use_reg(var_2, reg_2);
			get_result_reg(des, reg_3);
			new_mips(ADD_STR, reg_3, reg_1, reg_2);
		}
	}
	write_back(des, reg_3);
}
void C0_Compiler::mips_sub(mid_code* temp_midcode) {
	string* reg_1 = new string(*T7_STR);
	string* reg_2 = new string(*T8_STR);
	string* reg_3 = new string(*T9_STR);
	string* var_1 = temp_midcode->var_1;
	string* var_2 = temp_midcode->var_2;
	string* des = temp_midcode->des;

	if (var_is_number(var_1)) {
		get_use_reg(var_1, reg_1);
		get_use_reg(var_2, reg_2);
		get_result_reg(des, reg_3);
		new_mips(SUB_STR, reg_3, reg_1, reg_2);
	}
	else {
		get_use_reg(var_1, reg_1);
		if (var_is_number(var_2)) {
			get_result_reg(des, reg_3);
			new_mips(SUBI_STR, reg_3, reg_1, var_2);
		}
		else {
			get_use_reg(var_2, reg_2);
			get_result_reg(des, reg_3);
			new_mips(SUB_STR, reg_3, reg_1, reg_2);
		}
	}
	write_back(des, reg_3);
}

void C0_Compiler::mips_mult(mid_code* temp_midcode) {
	string* reg_1 = new string(*T7_STR);
	string* reg_2 = new string(*T8_STR);
	string* reg_3 = new string(*T9_STR);
	string* var_1 = temp_midcode->var_1;
	string* var_2 = temp_midcode->var_2;
	string* des = temp_midcode->des;

	get_use_reg(var_1, reg_1);
	get_use_reg(var_2, reg_2);
	get_result_reg(des, reg_3);

	new_mips(MULT_STR, reg_3, reg_1, reg_2);

	write_back(des, reg_3);
}
void C0_Compiler::mips_div(mid_code* temp_midcode) {
	string* reg_1 = new string(*T7_STR);
	string* reg_2 = new string(*T8_STR);
	string* reg_3 = new string(*T9_STR);
	string* var_1 = temp_midcode->var_1;
	string* var_2 = temp_midcode->var_2;
	string* des = temp_midcode->des;

	get_use_reg(var_1, reg_1);
	get_use_reg(var_2, reg_2);
	get_result_reg(des, reg_3);

	new_mips(DIV_STR, reg_1, reg_2);
	new_mips(MFLO_STR, reg_3);

	write_back(des, reg_3);
}
void C0_Compiler::mips_scanf(mid_code* temp_midcode) {
	if (*temp_midcode->var_2 == "int") {
		new_mips(LI_STR, V0_STR, 5);
	}
	else {
		new_mips(LI_STR, V0_STR, 12);
	}
	new_mips(SYSCALL_STR);

	string* reg_3 = new string(*T9_STR);
	get_result_reg(temp_midcode->des, reg_3);
	new_mips(ADDI_STR, reg_3, V0_STR, 0);
	write_back(temp_midcode->des, reg_3);
}
void C0_Compiler::mips_printf(mid_code* temp_midcode) { //var_2是类型 des是数据 var_1是换行
	string* reg_1 = new string(*T7_STR);
	string* reg_3 = new string(*T9_STR);
	string* var_1 = temp_midcode->var_1;
	string* des = temp_midcode->des;

	if (*temp_midcode->var_2 == string("string")) {
		stringstream sstring = stringstream();
		sstring << "$String" << *temp_midcode->des;
		new_mips(LA_STR, A0_STR, &sstring.str());
		new_mips(LI_STR, V0_STR, 4);
		new_mips(SYSCALL_STR);
	}
	else if (*temp_midcode->var_2 == string("int")) {
		get_use_reg(des, reg_3);
		new_mips(ADDI_STR, A0_STR, reg_3, 0);
		new_mips(LI_STR, V0_STR, 1);
		new_mips(SYSCALL_STR);
	}
	else {
		get_use_reg(des, reg_3);
		new_mips(ADDI_STR, A0_STR, reg_3, 0);
		new_mips(LI_STR, V0_STR, 11);
		new_mips(SYSCALL_STR);
	}
	//自行换行
	if (*temp_midcode->var_1 == string("\\n")) {
		new_mips(ADDI_STR, A0_STR, R0_STR, '\n'); //换行
		new_mips(LI_STR, V0_STR, 11);
		new_mips(SYSCALL_STR);
	}
}
void C0_Compiler::mips_exit(mid_code* temp_mid_code) {
	new_mips(LI_STR, V0_STR, 10);
	new_mips(SYSCALL_STR);
}

void C0_Compiler::mid_to_mips() {
	mid_code *temp_midcode;

	new_mips(new string(".text"));

	new_mips(ADD_STR, FP_STR, SP_STR, R0_STR);
	new_mips(ADDI_STR, GP_STR, GP_STR, 0x2000);

	mips_goto(new string("main"));

	int i;
	for (i = 0; i < mid_code_table_top; i++) {
		temp_midcode = mid_code_table[i];
		switch (temp_midcode->op) {
		case FUNCBEGINOP:
			current_mips_func += 1; // 初始化为-1, 0表示第一个
			mips_func_init(temp_midcode->des);
			break;
		case EQLEQLOP:
		case NEQOP:
		case GTROP:
		case GTREQLOP:
		case LSSOP:
		case LSSEQLOP:
			mips_branch(temp_midcode);
			break;

		case PARAVALOP:
			mips_paraval(temp_midcode->des);
			break;

		case CALLOP:
			mips_call(temp_midcode->des);
			break;
		case RETOP:
			mips_ret(temp_midcode->des);
			break;
		case ARRAYTOOP:
			mips_arrayto(temp_midcode);
			break;
		case TOARRAYOP:
			mips_toarray(temp_midcode);
			break;
		case ADDOP:
			mips_add(temp_midcode);
			break;
		case SUBOP:
			mips_sub(temp_midcode);
			break;
		case MULTOP:
			mips_mult(temp_midcode);
			break;
		case DIVOP:
			mips_div(temp_midcode);
			break;
		case LABELOP:
			new_mips_label(temp_midcode->des);
			break;
		case GOTOOP:
			mips_goto(temp_midcode->des);
			break;
		case SCANFOP:
			mips_scanf(temp_midcode);
			break;
		case PRINTFOP:
			mips_printf(temp_midcode);
			break;
		case EXITOP:
			mips_exit(temp_midcode);
			break;
		}
	}
}

void C0_Compiler::mips_generate() {
	ascii_init();
	mid_to_mips();
	output_mips_code();
	cout << "Mips Generate Over." << endl;
}