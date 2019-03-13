#include "C0_Compiler.h"

////cout << token_type_enum_to_string[current_token->t_type] << " \t" << current_token->line << " \t" << current_token->pos << endl;

void C0_Compiler::grammar_analysis() {
	sym_table_init();
	main_sym_table_init();
	next_symbol();
	program();
	print_token_table();
}

void C0_Compiler::program() {
	int temp_ret_type;
	int temp_sym_type;

	save_spot();

	if (current_token->t_type == CONST_SYMBOL) {
		const_decl();
		save_spot();
	}

	if (current_token->t_type == INT_SYMBOL || current_token->t_type == CHAR_SYMBOL) {

		next_symbol();

		if (current_token->t_type == IDENT) {
			next_symbol();
			if (current_token->t_type == COMMA || current_token->t_type == SEMICOLON
				|| current_token->t_type == LEFT_BRACKET) { //2018.12.07_13:25发现BUG
				//还可以是中括号
				back_spot();

				var_decl();
			}
			else {
				back_spot();
			}
		}
	}
	int mark_type;

	while (current_token->t_type == INT_SYMBOL || current_token->t_type == CHAR_SYMBOL || current_token->t_type == VOID_SYMBOL) {

		mark_type = current_token->t_type;
		save_spot();
		next_symbol();

		if (current_token->t_type != MAIN_SYMBOL) {
			if (mark_type == INT_SYMBOL || mark_type == CHAR_SYMBOL) {
				string* func_name = current_token->t_value.pstring;
				back_spot();
				ret_func_def(func_name);
			}
			else {
				string* func_name = current_token->t_value.pstring;
				back_spot();
				non_ret_func_def(func_name);
			}

		}
		else {
			back_spot();
			main_def(new string("main"));
		}
	}
}

ret_type C0_Compiler::type_ident() {
	ret_type result;
	if (current_token->t_type != INT_SYMBOL && current_token->t_type != CHAR_SYMBOL)
		ERROR(-1);
	else {
		if (current_token->t_type == INT_SYMBOL)
			result = INTRET;
		else
			result = CHARRET;

		next_symbol();
		return result;
	}
}

string* C0_Compiler::ident() {
	string *result;
	if (current_token->t_type != IDENT)
		ERROR(-1);
	else {
		result = current_token->t_value.pstring;
		next_symbol();
		return result;
	}
}

void C0_Compiler::signed_int(int *para_length_number) {
	bool is_neg = false;
	if (current_token->t_type == PLUS || current_token->t_type == SUB) {
		if (current_token->t_type == SUB)
			is_neg = true;
		next_symbol();
	}
	if (current_token->t_type == INT) {
		*para_length_number = current_token->t_value.number;
		if (is_neg)
			*para_length_number = -*para_length_number;
		////cout << "This is Signed Int." << endl;
		next_symbol();
	}
	else
		ERROR(-1);
}

void C0_Compiler::const_decl() {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	do {

		if (current_token->t_type != CONST_SYMBOL)
			ERROR(-1);
		else {
			next_symbol();
			const_def();
			if (current_token->t_type != SEMICOLON) {
				ERROR(-1);
			}
			else {
				next_symbol();
			}
		}
	} while (current_token->t_type == CONST_SYMBOL);
	//cout << temp_line << " \t" << temp_pos << " :\t";
	//cout << "This is Const Declare. Const Definition is Over." << endl;
}

void C0_Compiler::const_def() {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	symbol_i_type = CONSTIDENT;

	if (current_token->t_type != INT_SYMBOL && current_token->t_type != CHAR_SYMBOL) {
		ERROR(-1);
	}
	else if (current_token->t_type == INT_SYMBOL) {
		symbol_r_type = INTRET;
		
		do {
			next_symbol();
			if (current_token->t_type != IDENT)
				ERROR(-1);
			else {
				symbol_name = current_token->t_value.pstring;
				next_symbol();
				if (current_token->t_type != BECOME)
					ERROR(-1);
				else {
					next_symbol();
					signed_int(&symbol_para_length_number);
					
					current_symbol = new_symbol(symbol_name, symbol_para_length_number, current_func, symbol_i_type, symbol_r_type);
					if (find_symbol(symbol_name, current_func, localed) == nullptr) {
						add_sym_table(current_symbol);
					}
					else {
						ERROR(-1);//ERROR
					}
				}
			}
		} while (current_token->t_type == COMMA);
	}
	else {// current_token->t_type == CHAR_SYMBOL
		symbol_r_type = CHARRET;
		do {
			next_symbol();
			if (current_token->t_type != IDENT)
				ERROR(-1);
			else {
				symbol_name = current_token->t_value.pstring;

				next_symbol();
				if (current_token->t_type != BECOME)
					ERROR(-1);
				else {
					next_symbol();
					if (current_token->t_type != CHAR)
						ERROR(-1);
					else {
						symbol_para_length_number = current_token->t_value.letter;
						
						current_symbol = new_symbol(symbol_name, symbol_para_length_number, current_func, symbol_i_type, symbol_r_type);
						if (find_symbol(symbol_name, current_func, localed) == nullptr) {
							add_sym_table(current_symbol);
						}
						else {
							ERROR(-1);
						}
						
						next_symbol();
					}
				}
			}
		} while (current_token->t_type == COMMA);
	}
	//cout << temp_line << " \t" << temp_pos << " :\t";
	//cout << "This is Const Definition." << endl;
}

void C0_Compiler::declare_head() {
	if (current_token->t_type == INT_SYMBOL) {
		next_symbol();
		if (current_token->t_type == IDENT)
			//cout << "This is Declare Head." << endl;
			;
		else {
			ERROR(-1);
		}
		next_symbol();
	}
	else if (current_token->t_type == CHAR_SYMBOL) {
		next_symbol();
		if (current_token->t_type == IDENT)
			//cout << "this is Declare Head." << endl;
			;
		else
			ERROR(-1);
		next_symbol();
	}
	else {
		ERROR(-1);
	}
}

void C0_Compiler::var_decl() {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	do {
		var_def();
		if (current_token->t_type != SEMICOLON) {
			ERROR(-1);
		}
		else {

			next_symbol();

			save_spot();

			if (current_token->t_type == INT_SYMBOL || current_token->t_type == CHAR_SYMBOL) {
				next_symbol();

				if (current_token->t_type == IDENT) {
					next_symbol();
					if (current_token->t_type == SEMICOLON || current_token->t_type == COMMA
						|| current_token->t_type == LEFT_BRACKET) {
						back_spot();
						continue;
					}
					else {
						back_spot();
						break;
					}
				}
			}
			/////
		}
	} while (current_token->t_type == INT_SYMBOL || current_token->t_type == CHAR_SYMBOL);

	//cout << "This is Var Declare. Var Definition is Over." << endl;
}

void C0_Compiler::var_def() {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	symbol_i_type = VARIDENT;

	if (current_token->t_type == INT_SYMBOL || current_token->t_type == CHAR_SYMBOL) {
		if (current_token->t_type == INT_SYMBOL)
			symbol_r_type = INTRET;
		else
			symbol_r_type = CHARRET;

		do {
			next_symbol();

			if (current_token->t_type == IDENT) {
				symbol_name = current_token->t_value.pstring;

				next_symbol();
				if (current_token->t_type == LEFT_BRACKET) {
					symbol_i_type = ARRAYIDENT;

					next_symbol();
					if (current_token->t_type == INT) {
						symbol_para_length_number = current_token->t_value.number;
						if (symbol_para_length_number <= 0)
							ERROR(-1);
						//大于0判断
						next_symbol();
						if (current_token->t_type == RIGHT_BRACKET) {
							next_symbol();

							current_symbol = new_symbol(symbol_name, symbol_para_length_number, current_func, symbol_i_type, symbol_r_type);
							if (find_symbol(symbol_name, current_func, localed) == nullptr) {
								add_sym_table(current_symbol);
							}
							else
								ERROR(-1);
						}
						else
							ERROR(-1);
					}
					else
						ERROR(-1);
				}
				else {
					symbol_para_length_number = -1;
					current_symbol = new_symbol(symbol_name, symbol_para_length_number, current_func, symbol_i_type, symbol_r_type);
					if (find_symbol(symbol_name, current_func, localed) == nullptr) {
						add_sym_table(current_symbol);
					}
					else
						ERROR(-1);
				}
			}
			else
				ERROR(-1);
		} while (current_token->t_type == COMMA);
		//cout << temp_line << " \t" << temp_pos << " :\t";
		//cout << "This is Var Def." << endl;
	}
	else
		ERROR(-1);
}

void C0_Compiler::ret_func_def(string* func_name) {
	int i;
	this->return_state_number = 0;
	ret_type func_ret_temp;

	symbol_i_type = FUNCIDENT;

	

	if (current_token->t_type != INT_SYMBOL && current_token->t_type != CHAR_SYMBOL)
		ERROR(-1);
	else {
		if (current_token->t_type == INT_SYMBOL)
			func_ret_temp = symbol_r_type = INTRET;
		else
			func_ret_temp = symbol_r_type = CHARRET;

		new_add_mid_code(FUNCBEGINOP, func_ret_temp == INTRET ? new string("int") : new string("char")
			, new string(), func_name);

		next_symbol();
		if (current_token->t_type != IDENT)
			ERROR(-1);
		else {
			symbol_name = current_token->t_value.pstring;

			next_symbol();

			current_symbol = new_symbol(symbol_name, 0, current_func, symbol_i_type, symbol_r_type);

			if (find_symbol(symbol_name, current_func, localed) == nullptr) {
				add_sym_table(current_symbol);
				push_func();
			}
			else
				ERROR(-1);

			if (current_token->t_type == LEFT_PAR) {
				next_symbol();

				para_list(current_symbol);
				if (current_token->t_type != RIGHT_PAR) {
					ERROR(-1);
				}
				else {
					next_symbol();
				}
			}
			if (current_token->t_type != LEFT_BRACE)
				ERROR(-1);
			else {
				next_symbol();
				
				complex_state(func_name);


				if (this->return_state_number == 0)
					ERROR(-1);//ret_func没有返回语句

				for (i = 0; i < this->return_state_number; i++) {
					if (this->return_state_list[i] != func_ret_temp) {
						ERROR(-1); //返回值类型不匹配
						break;
					}
				}

				if (current_token->t_type != RIGHT_BRACE)
					ERROR(-1);
				else {
					next_symbol();
					//cout << "This is Return Function Definition. Over." << endl;
				}
			}
		}
	}
	string* temp_string = new string(*func_name);
	temp_string->append("_end");
	new_add_mid_code(LABELOP, new string(), new string(), temp_string);
	new_add_mid_code(RETOP, new string(), new string(), func_name);

	pop_func();
}

void C0_Compiler::non_ret_func_def(string* func_name) {
	int i;
	this->return_state_number = 0;
	ret_type func_ret_temp;

	new_add_mid_code(FUNCBEGINOP, new string("void"), new string(), func_name);

	symbol_i_type = FUNCIDENT;

	if (current_token->t_type != VOID_SYMBOL)
		ERROR(-1);
	else {
		func_ret_temp = symbol_r_type = VOIDRET;

		next_symbol();
		if (current_token->t_type != IDENT)
			ERROR(-1);
		else {
			symbol_name = current_token->t_value.pstring;

			next_symbol();

			current_symbol = new_symbol(symbol_name, 0, current_func, symbol_i_type, symbol_r_type);
			if (find_symbol(symbol_name, current_func, localed) == nullptr) {
				add_sym_table(current_symbol);
				push_func();
			}
			else {
				ERROR(-1);
			}
			
			if (current_token->t_type == LEFT_PAR) {
				next_symbol();

				
				para_list(current_symbol);
				if (current_token->t_type != RIGHT_PAR)
					ERROR(-1);
				else {
					next_symbol();
				}
			}

			if (current_token->t_type != LEFT_BRACE)
				ERROR(-1);
			else {
				next_symbol();
				complex_state(func_name);

				for (i = 0; i < this->return_state_number; i++) {
					if (this->return_state_list[i] != func_ret_temp) {
						ERROR(-1); //返回值类型不匹配
						break;
					}
				}

				if (current_token->t_type != RIGHT_BRACE)
					ERROR(-1);
				else {
					next_symbol();

					//cout << "This is Return Function Definition. Over" << endl;
				}
			}
		}
	}
	string* temp_string = new string(*func_name);
	temp_string->append("_end");
	new_add_mid_code(LABELOP, new string(), new string(), temp_string);
	new_add_mid_code(RETOP, new string(), new string(), func_name);

	pop_func();
}

void C0_Compiler::complex_state(string* func_name) {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	if (current_token->t_type == CONST_SYMBOL) {
		const_decl();
	}

	if (current_token->t_type == INT_SYMBOL || current_token->t_type == CHAR_SYMBOL) {
		var_decl();
	}
	statement_list(func_name);
	//cout << temp_line << " \t" << temp_pos << " :\t";
	//cout << "This is Complex Statements. Over." << endl;
}

void C0_Compiler::para_list(symbol* func_symbol) {
	int temp_line, temp_pos;
	int count = 0;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	func_symbol->para_list[count] = symbol_r_type =  type_ident();
	symbol_name = ident();
	symbol_i_type = PARAIDENT;
	
	current_symbol = new_symbol(symbol_name, -1, current_func, symbol_i_type, symbol_r_type);

	if (find_symbol(symbol_name, current_func, localed) == nullptr) {
		add_sym_table(current_symbol);
	}
	else
		ERROR(-1);

	count++;

	while (current_token->t_type == COMMA) {
		next_symbol();
		func_symbol->para_list[count] = symbol_r_type = type_ident();
		symbol_name = ident();
		symbol_i_type = PARAIDENT;

		current_symbol = new_symbol(symbol_name, -1, current_func, symbol_i_type, symbol_r_type);

		if (find_symbol(symbol_name, current_func, localed) == nullptr) {
			add_sym_table(current_symbol);
		}
		else
			ERROR(-1);

		count++;
	}
	func_symbol->number = count;
	//cout << "This is Para List." << endl;
}

void C0_Compiler::main_def(string* func_name) {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	symbol* temp_symbol = new_symbol(func_name, 0, current_func, FUNCIDENT, VOIDRET);
	add_sym_table(temp_symbol);
	new_add_mid_code(FUNCBEGINOP, new string("void"), new string(), func_name);

	if (current_token->t_type != VOID_SYMBOL)
		ERROR(-1);
	else {
		next_symbol();
		if (current_token->t_type != MAIN_SYMBOL)
			ERROR(-1);
		else {
			next_symbol();
			if (current_token->t_type != LEFT_PAR)
				ERROR(-1);
			else {
				next_symbol();
				if (current_token->t_type != RIGHT_PAR)
					ERROR(-1);
				else {
					next_symbol();
					if (current_token->t_type != LEFT_BRACE)
						ERROR(-1);
					else {
						next_symbol();
						push_func();
						complex_state(func_name);
						if (current_token->t_type != RIGHT_BRACE)
							ERROR(-1);
						else {
							new_add_mid_code(EXITOP, new string(), new string(), new string());
							//next_symbol(); //???????你是傻屌嘛
							//cout << temp_line << " \t" << temp_pos << " :\t";
							//cout << "This is Main Definition. Over." << endl;
						}
					}
				}
			}
		}
	}
	pop_func();
	cout << sym_table_top << endl;
	cout << "Grammar Analysis is OVER!" << endl;
}

ret_type C0_Compiler::expression(string* var_name) {
	bool is_charret = true;
	ret_type temp;

	string* var_1 = new string();
	bool neg = false;

	if (current_token->t_type == PLUS) {
		next_symbol();
		is_charret = false;
	}
	else if (current_token->t_type == SUB) {
		next_symbol();
		is_charret = false;
		neg = true;
	}
	temp = term(var_1);

	if (neg) {
		if (var_is_number(var_1)) {//如果是一个数字常量的话
			int val = atoi(var_1->c_str());
			val = val * -1;
			string* constvalue = new string();
			constvalue->append(to_string(val));

			var_1 = constvalue;

		}
		else if (var_is_ident(var_1)) {//如果是一个标识符
			string* temp_ident = new string();
			temp_ident = new_temp(); //?临时变量从不从符号表里删除？？？？？？

			new_add_mid_code(SUBOP, ZERO_VAR, var_1, temp_ident);

			var_1 = temp_ident;

			
		}
		else {//临时变量
			new_add_mid_code(SUBOP, ZERO_VAR, var_1, var_1);
		}
	}
	

	while (current_token->t_type == PLUS || current_token->t_type == SUB) {
		bool is_plus = current_token->t_type == PLUS;

		string* var_2 = new string();

		next_symbol();
		term(var_2);
		is_charret = false;

		if (var_is_number(var_1)) {
			if (var_is_number(var_2)) {
				int val_1 = atoi(var_1->c_str());
				int val_2 = atoi(var_2->c_str());
				int result = is_plus ? val_1 + val_2 : val_1 - val_2;
				string* constvalue = new string();
				constvalue->append(to_string(result));

				var_1 = constvalue;
			}
			else {
				if (var_is_ident(var_2)) { //var2是临时变量的话，需要用临时变量存
					string* temp_ident;
					temp_ident = new_temp();
					new_add_mid_code(is_plus ? ADDOP : SUBOP, var_1, var_2, temp_ident);

					var_1 = temp_ident;
					
				}
				else { //
					new_add_mid_code(is_plus ? ADDOP : SUBOP, var_1, var_2, var_2);
					var_1 = var_2;
				}
			}
		}
		else {
			if (var_is_temp(var_1)) {
				new_add_mid_code(is_plus ? ADDOP : SUBOP, var_1, var_2, var_1);

			}
			else {
				string* temp_ident = new string();
				temp_ident = new_temp();
				new_add_mid_code(is_plus ? ADDOP : SUBOP, var_1, var_2, temp_ident);
				var_1 = temp_ident;
			}
		}
	}
	*var_name = *var_1;

	if (is_charret && temp == CHARRET)
		return CHARRET;
	else
		return INTRET;

	//cout << "This is An Expression." << endl;
}

ret_type C0_Compiler::term(string* var_name) {
	bool is_charret = true;
	ret_type temp;

	string* result_var = new string(); //代替var_1

	temp = factor(result_var);

	if (var_is_ret(result_var)) { //如果是#ret，则需要临时变量
		string* temp_ident;
		temp_ident = new_temp();
		new_add_mid_code(ADDOP, result_var, ZERO_VAR, temp_ident);
		result_var = temp_ident;
	}

	while (current_token->t_type == MULT || current_token->t_type == DIV) {
		bool is_mult = current_token->t_type == MULT;
		string *var_2 = new string();

		next_symbol();
		factor(var_2);

		if (var_is_number(result_var)) {
			if (var_is_number(var_2)) {
				int val_1 = atoi(result_var->c_str());
				int val_2 = atoi(var_2->c_str());
				if (val_2 == 0 && !is_mult) {//除0报错
					ERROR(-1);
				}
				int result = is_mult ? val_1 * val_2 : val_1 / val_2;
				string* constvalue = new string();
				constvalue->append(to_string(result));

				result_var = constvalue;
			}
			else {
				if (!var_is_temp(var_2)) { //如果不是临时变量
					string* temp_ident = new string();
					temp_ident = new_temp();
					new_add_mid_code(is_mult ? MULTOP : DIVOP, result_var, var_2, temp_ident);
					result_var = temp_ident;
				}
				else {//是临时变量
					new_add_mid_code(is_mult ? MULTOP : DIVOP, result_var, var_2, var_2);
					result_var = var_2;
				}
			}
		}
		else {
			if (var_is_number(var_2)) {
				int val_2 = atoi(var_2->c_str());
				if (val_2 == 0 && is_mult) {//不能除0
					ERROR(-1);
				}
			}
			if (var_is_temp(result_var)) { // ??有疑问
				new_add_mid_code(is_mult ? MULTOP : DIVOP, result_var, var_2, result_var);
			}
			else {
				//if (var_is_temp(var_2)) {//不能给var_2当做最终的临时变量，因为最终把reg_1的结果给var_name
				//	new_add_mid_code(is_mult ? MULTOP : DIVOP, result_var, var_2, var_2);
				//}
				//else { //都不是临时变量 
					string* temp_ident;
					temp_ident = new_temp();
					new_add_mid_code(is_mult ? MULTOP : DIVOP, result_var, var_2, temp_ident);

					result_var = temp_ident;
				//}
			}
		}
		

		is_charret = false;
	}
	*var_name = *result_var;

	if (is_charret && temp == CHARRET)
		return CHARRET;
	else
		return INTRET;
}

ret_type C0_Compiler::factor(string* var_name) {
	bool is_charret = true;

	if (current_token->t_type == IDENT) {

		//if(current_token) //判断是函数还是变量常量数组*************************************
		ident_type sym_itype;
		string *sym_name;
		symbol *sym_temp;

		sym_name = current_token->t_value.pstring;
		sym_temp = find_symbol(sym_name, current_func, not_localed);

		if (sym_temp == nullptr) {
			ERROR(-1);//没有声明
		}
		else
			sym_itype = sym_temp->i_type;
			   
		if (sym_temp->r_type == INTRET)
			is_charret = false;

		save_spot();
		next_symbol();

		if (current_token->t_type == LEFT_PAR) {
			if (sym_itype != FUNCIDENT)
				ERROR(-1);//不是函数报错
			back_spot();
			ret_func_call(sym_temp);
			
			*var_name = string("#RET");
		}
		else if (current_token->t_type == LEFT_BRACKET) {
			string* array_index = new string();

			if (sym_itype != ARRAYIDENT)
				ERROR(-1); //不是数组报错

			next_symbol();
			expression(array_index);

			*var_name = *new_temp();
			new_add_mid_code(ARRAYTOOP, sym_temp->name, array_index, var_name);


			if (current_token->t_type != RIGHT_BRACKET)
				ERROR(-1);
			else
				next_symbol();
					
		}
		else {
			back_spot();
			if (sym_itype == FUNCIDENT) {
				ret_func_call(sym_temp);
				*var_name = string("#RET");
			}
			else if (sym_itype == VARIDENT || sym_itype == PARAIDENT) {
				ident();
				*var_name = *sym_temp->name;
			}
			else if (sym_itype == CONSTIDENT) {
				ident();
				string* constvalue = new string(to_string(sym_temp->number));
				*var_name = *constvalue;
				
			}
			else {
				ERROR(-1);//不是函数或变量报错
			}
		}
	}
	else if (current_token->t_type == PLUS || current_token->t_type == SUB || current_token->t_type == INT) {
		bool neg = false;
		if (current_token->t_type != INT) {
			neg = current_token->t_type == SUB;
			next_symbol();
		}

		string* constvalue = new string();
		if(neg)
			constvalue->append(to_string(current_token->t_value.number * -1));
		else
			constvalue->append(to_string(current_token->t_value.number));
		*var_name = *constvalue;

		next_symbol();
		is_charret = false;
	}
	else if (current_token->t_type == CHAR) {
		string* constvalue = new string();
		constvalue->append(to_string(current_token->t_value.letter));
		*var_name = *constvalue;

		next_symbol();
	}
	else if (current_token->t_type == LEFT_PAR) {
		next_symbol();
		expression(var_name);
		if (current_token->t_type != RIGHT_PAR) {
			ERROR(-1);
		}
		else
			next_symbol();
		is_charret = false;
	}
	if (is_charret)
		return CHARRET;
	else
		return INTRET;
}

void C0_Compiler::statement(string* func_name) {
	symbol *sym_temp;
	string *sym_name;

	if (current_token->t_type == IF_SYMBOL) { // OK
		if_state(func_name);
		
	}
	else if (current_token->t_type == DO_SYMBOL) { // OK
		dowhile_state(func_name);
	}
	else if (current_token->t_type == SWITCH_SYMBOL) { // OK
		switch_state(func_name);
	}
	else if (current_token->t_type == IDENT) {
		
		sym_name = current_token->t_value.pstring;
		sym_temp = find_symbol(sym_name, current_func, not_localed);
		
		if (sym_temp == nullptr) {
			ERROR(-1);//未定义错误
		}

		save_spot();
		next_symbol();


		if (current_token->t_type == BECOME || current_token->t_type == LEFT_BRACKET) {
			back_spot();
			become_state();
		}
		else if (current_token->t_type == LEFT_PAR) {
			back_spot();
			if (sym_temp->i_type != FUNCIDENT) {
				ERROR(-1);//不是函数报错
			}
			else {
				ret_func_call(sym_temp);
				//new_add_mid_code(CALLOP, new string(), new string(), sym_temp->name);
			}
		}
		else {
			back_spot();
			if (sym_temp->i_type != FUNCIDENT)
				ERROR(-1);
			else {
				non_ret_func_call(sym_temp);
				//new_add_mid_code(CALLOP, new string(), new string(), sym_temp->name);
			}
		}
		if (current_token->t_type != SEMICOLON) {
			ERROR(-1);
		}
		else {
			next_symbol();
		}
	}
	else if (current_token->t_type == SCANF_SYMBOL) {
		scanf_state();
		if (current_token->t_type != SEMICOLON)
			ERROR(-1);
		else
			next_symbol();
	}
	else if (current_token->t_type == PRINTF_SYMBOL) {
		printf_state();
		if (current_token->t_type != SEMICOLON)
			ERROR(-1);
		else
			next_symbol();
	}
	else if (current_token->t_type == RETURN_SYMBOL) {
		ret_state(func_name);

		if (current_token->t_type != SEMICOLON) {
			ERROR(-1);
		}
		else
			next_symbol();
	}
	else if (current_token->t_type == SEMICOLON) // OK
		next_symbol();

	else if (current_token->t_type == LEFT_BRACE) { // 基本OK 可能传参
		next_symbol();
		statement_list(func_name);
		if (current_token->t_type != RIGHT_BRACE)
			ERROR(-1);
		else {
			next_symbol();
		}
	}
	else {
		
		ERROR(-1);
	}
}

void C0_Compiler::become_state() {
	symbol *sym_temp;
	string *sym_name;
	ret_type exp_ret_type;

	if (current_token->t_type != IDENT)
		ERROR(-1);
	else {
		sym_name = current_token->t_value.pstring;
		sym_temp = find_symbol(sym_name, current_func, not_localed);

		if (sym_temp == nullptr)
			ERROR(-1);//未定义错误

		next_symbol();
		if (current_token->t_type == BECOME) {
			if (sym_temp->i_type != PARAIDENT && sym_temp->i_type != VARIDENT)
				ERROR(-1);//只有变量和参量可以赋值
			next_symbol();

			string* temp_ident = new string();

			exp_ret_type = expression(temp_ident);

			if (var_is_temp(temp_ident)) {

				mid_code* temp_code;
				temp_code = get_mid_code(mid_code_table_top - 1);

				*temp_code->des = *sym_temp->name; //有BUG！不仅改了我的中间代码，还改了我的符号表！

			}
			else {
				new_add_mid_code(ADDOP, temp_ident, ZERO_VAR, sym_temp->name);
			}

			if (sym_temp->r_type != exp_ret_type)
				ERROR(-1);//赋值类型不匹配
		}
		else if (current_token->t_type == LEFT_BRACKET) {
			if (sym_temp->i_type != ARRAYIDENT)
				ERROR(-1);//不是数组报错
			
			next_symbol();

			string* temp_index = new string();
			expression(temp_index);
			

			if (current_token->t_type != RIGHT_BRACKET)
				ERROR(-1);
			else {
				next_symbol();
				if (current_token->t_type != BECOME)
					ERROR(-1);
				else {
					next_symbol();

					string* temp_ident = new string();

					exp_ret_type = expression(temp_ident);

					if (sym_temp->r_type != exp_ret_type)
						ERROR(-1);//数组赋值不匹配

					new_add_mid_code(TOARRAYOP, temp_ident, temp_index, sym_name);
				}
			}
		}
		else
			ERROR(-1);
	}
	//cout << "This is Become Statement." << endl;
}

void C0_Compiler::if_state(string* func_name) {

	if (current_token->t_type != IF_SYMBOL)
		ERROR(-1);
	else {
		next_symbol();
		if (current_token->t_type != LEFT_PAR)
			ERROR(-1);
		else {
			next_symbol();

			string* else_label;
			string* end_label;
			else_label = new_label();
			end_label = new_label();

			condition(else_label);

			if (current_token->t_type != RIGHT_PAR)
				ERROR(-1);
			else {
				next_symbol();
				statement(func_name);

				new_add_mid_code(GOTOOP, new string(), new string(), end_label);
				new_add_mid_code(LABELOP, new string(), new string(), else_label);

				if (current_token->t_type != ELSE_SYMBOL)
					ERROR(-1);
				else {
					next_symbol();
					statement(func_name);

					new_add_mid_code(LABELOP, new string(), new string(), end_label);
				}
			}
		}
	}
	//cout << "This is If Statement." << endl;
}

void C0_Compiler::condition(string* label_name) {

	ret_type exp_type_1, exp_type_2;
	bool two_expression = false;
	string* var_1 = new string();
	string* var_2 = new string();
	token_type relate_temp;

	exp_type_1 = expression(var_1);

	if (current_token->t_type != GTR && current_token->t_type != GTREQL
		&& current_token->t_type != LSS && current_token->t_type != LSSEQL
		&& current_token->t_type != EQLEQL && current_token->t_type != NEQ)
		;
	else {
		
		relate_temp = current_token->t_type;
		
		next_symbol();
		exp_type_2 = expression(var_2);
		two_expression = true;
	}
	if (two_expression && exp_type_1 == exp_type_2) { //不满足Case情况才跳转..所以生成的中间代码是相反的
		
		switch (relate_temp) {
		case EQLEQL:
			new_add_mid_code(NEQOP, var_1, var_2, label_name);
			break;
		case NEQ:
			new_add_mid_code(EQLEQLOP, var_1, var_2, label_name);
			break;
		case GTR:
			new_add_mid_code(LSSEQLOP, var_1, var_2, label_name);
			break;
		case GTREQL:
			new_add_mid_code(LSSOP, var_1, var_2, label_name);
			break;
		case LSS:
			new_add_mid_code(GTREQLOP, var_1, var_2, label_name);
			break;
		case LSSEQL:
			new_add_mid_code(GTROP, var_1, var_2, label_name);
			break;
			
		}
	}
	else if (!two_expression && exp_type_1 == INTRET) {
		new_add_mid_code(EQLEQLOP, var_1, ZERO_VAR, label_name);
	}

	else
		ERROR(-1);//条件判断两边不匹配
	
	//cout << "This is Condition." << endl;
}

void C0_Compiler::dowhile_state(string* func_name) {
	string* while_label;
	string* end_label;
	while_label = new_label();
	end_label = new_label();

	if (current_token->t_type != DO_SYMBOL)
		ERROR(-1);
	else {
		new_add_mid_code(LABELOP, new string(), new string(), while_label);

		next_symbol();
		statement(func_name); //?
		if (current_token->t_type != WHILE_SYMBOL)
			ERROR(-1);
		else {
			next_symbol();
			if (current_token->t_type != LEFT_PAR)
				ERROR(-1);
			else {
				next_symbol();
				condition(end_label);//??
				if (current_token->t_type != RIGHT_PAR)
					ERROR(-1);
				else {
					next_symbol();
				}
			}
		}
		new_add_mid_code(GOTOOP, new string(), new string(), while_label);
		new_add_mid_code(LABELOP, new string(), new string(), end_label);
	}
	//cout << "This is Do While Statement." << endl;
}

int C0_Compiler::const_value() {
	int result;
	if (current_token->t_type != INT && current_token->t_type != CHAR)
		ERROR(-1);
	else {
		if (current_token->t_type == INT)
			result = current_token->t_value.number;
		else
			result = current_token->t_value.letter;

		next_symbol();
		return result;
	}
}

void C0_Compiler::switch_state(string* func_name) {

	if (current_token->t_type != SWITCH_SYMBOL)
		ERROR(-1);
	else {
		next_symbol();
		if (current_token->t_type != LEFT_PAR)
			ERROR(-1);
		else {
			string* switch_name = new string();

			next_symbol();
			expression(switch_name);

			string* done_label;
			done_label = new_label();

			if (current_token->t_type != RIGHT_PAR)
				ERROR(-1);
			else {
				next_symbol();
				if (current_token->t_type != LEFT_BRACE)
					ERROR(-1);
				else {
					next_symbol();


					case_list(done_label, func_name, switch_name); // 传参
					default_state(func_name, switch_name); // ??
					
					new_add_mid_code(LABELOP, new string(), new string(), done_label);

					if (current_token->t_type != RIGHT_BRACE)
						ERROR(-1);
					else {
						next_symbol();
					}
				}
			}
		}
	}
	//cout << "This is Switch Statement." << endl;
}

void C0_Compiler::case_list(string* done_label, string* func_name,string* switch_name) {
	int i;
	int count = 0;
	int temp;
	bool mark;
	int const_list[200];

	do {
		string* next_label;
		next_label = new_label();

		temp = case_state(next_label, done_label, func_name, switch_name);

		new_add_mid_code(LABELOP, new string(), new string(), next_label);

		mark = false;
		for (i = 0; i < count; i++) {
			if (temp == const_list[i]) {
				ERROR(-1);
				mark = true;
			}
		}
		if (mark == false) {
			const_list[count] = temp;
			count++;
		}

	} while (current_token->t_type == CASE_SYMBOL);
}

int C0_Compiler::case_state(string* next_label, string* done_label, string* func_name, string* switch_name) {
	int result;

	if (current_token->t_type != CASE_SYMBOL)
		ERROR(-1);
	else {
		next_symbol();
		result = const_value();

		string* constvalue = new string();
		string* temp_ident;
		constvalue->append(to_string(result));
		temp_ident = new_temp();
		new_add_mid_code(ADDOP, constvalue, ZERO_VAR, temp_ident);
		new_add_mid_code(NEQOP, switch_name, temp_ident,  next_label);

		if (current_token->t_type != COLON)
			ERROR(-1);
		else {
			next_symbol();
			statement(func_name);

			new_add_mid_code(GOTOOP, new string(), new string(), done_label);
		}
	}
	return result;
	//cout << "This is Case State." << endl;
}

void C0_Compiler::default_state(string* func_name, string* switch_name) {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	if (current_token->t_type != DEFAULT_SYMBOL)
		ERROR(-1);
	else {
		next_symbol();
		if (current_token->t_type != COLON)
			ERROR(-1);
		else {
			next_symbol();
			statement(func_name);
		}
	}
	//cout << temp_line << " \t" << temp_pos << " :\t";
	//cout << "This is Default Statement." << endl;
}

void C0_Compiler::ret_func_call(symbol* func_sym) {
	int para_number;
	para_number = func_sym->number;

	ident();

	if (current_token->t_type != LEFT_PAR) {
		if (para_number != 0)
			ERROR(-1);//函数参数不对
		new_add_mid_code(CALLOP, new string(), new string(), func_sym->name);
	}
	else {
		next_symbol();
		val_para_list(func_sym);
		if (current_token->t_type != RIGHT_PAR)
			ERROR(-1);
		else {
			new_add_mid_code(CALLOP, new string(), new string(), func_sym->name);
			next_symbol();
		}
	}

	//cout << "This is Return Function Call." << endl;
}

void C0_Compiler::non_ret_func_call(symbol* func_sym) {
	int para_number;
	para_number = func_sym->number;

	ident();

	if (current_token->t_type != LEFT_PAR) {
		if (para_number != 0)
			ERROR(-1);//函数参数不对
		new_add_mid_code(CALLOP, new string(), new string(), func_sym->name);
	}
	else {
		next_symbol();
		val_para_list(func_sym);
		if (current_token->t_type != RIGHT_PAR)
			ERROR(-1);
		else {
			next_symbol();
			new_add_mid_code(CALLOP, new string(), new string(), func_sym->name);
		}
	}
	//cout << "This is Non Return Function Call." << endl;
}

void C0_Compiler::val_para_list(symbol* func_sym) {
	int i;
	int count = 0;
	int para_number;
	ret_type para_list[50];
	para_number = func_sym->number;
	
	string* temp_para = new string();


	para_list[count] = expression(temp_para);
	new_add_mid_code(PARAVALOP, temp_para, ZERO_VAR, temp_para);

	count++;
	while (current_token->t_type == COMMA) {
		temp_para = new string();
		next_symbol();
		para_list[count] = expression(temp_para);
		new_add_mid_code(PARAVALOP, temp_para, ZERO_VAR, temp_para);

		count++;
	}
	if (para_number == count) {
		for (i = 0; i < para_number; i++) {
			if (para_list[i] == func_sym->para_list[i])
				;
			else {
				
				ERROR(-1);//参数种类不匹配
			}
		}
	}
	else {
		ERROR(-1);//参数数量不匹配
	}
	//cout << "This is Val Para List." << endl;
}

void C0_Compiler::statement_list(string* func_name) {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	while (current_token->t_type != RIGHT_BRACE) {
		statement(func_name);
	}
	//cout << temp_line << " \t" << temp_pos << " :\t";
	//cout << "This is Statement List. Over." << endl;
}

void C0_Compiler::scanf_state() {
	string *sym_name;
	symbol *sym_temp;

	if (current_token->t_type != SCANF_SYMBOL)
		ERROR(-1);
	else {
		next_symbol();
		if (current_token->t_type != LEFT_PAR)
			ERROR(-1);
		else {
			next_symbol();
			sym_name = ident();
			
			sym_temp = find_symbol(sym_name, current_func, not_localed);
			if (sym_temp == nullptr)
				ERROR(-1);//标识符不存在

			switch (sym_temp->r_type) {
			case INTRET:
				new_add_mid_code(SCANFOP, sym_temp->name, new string("int"), sym_temp->name);//??var_1可以尝试空串
				break;
			case CHARRET:
				new_add_mid_code(SCANFOP, sym_temp->name, new string("char"), sym_temp->name);
			}

			while (current_token->t_type == COMMA) {
				next_symbol();
				sym_name = ident();

				sym_temp = find_symbol(sym_name, current_func, not_localed);
				if (sym_temp == nullptr)
					ERROR(-1);//标识符不存在

				switch (sym_temp->r_type) {
				case INTRET:
					new_add_mid_code(SCANFOP, sym_temp->name, new string("int"), sym_temp->name);//??var_1可以尝试空串
					break;
				case CHARRET:
					new_add_mid_code(SCANFOP, sym_temp->name, new string("char"), sym_temp->name);
				}
			}

			if (current_token->t_type != RIGHT_PAR)
				ERROR(-1);
			else {
				next_symbol();
			}
		}
	}

	//cout << "This is Scanf Statement." << endl;
}

void C0_Compiler::printf_state() {

	if (current_token->t_type != PRINTF_SYMBOL)
		ERROR(-1);
	else {
		next_symbol();
		if (current_token->t_type != LEFT_PAR)
			ERROR(-1);
		else {
			next_symbol();
			if (current_token->t_type == STRING) { //var_1 大N不换行 小n换行
				int string_index;
				push_string(current_token->t_value.pstring, &string_index);
				string* string_index_str = new string();
				string_index_str->append(to_string(string_index));
				new_add_mid_code(PRINTFOP, new string("\\n"), new string("string"), string_index_str);

				next_symbol();
				if (current_token->t_type != COMMA && current_token->t_type != RIGHT_PAR)
					ERROR(-1);
				else if (current_token->t_type == COMMA) {
					mid_code* temp_midcode = mid_code_table[mid_code_table_top - 1];
					*temp_midcode->var_1 = "\\N";

					string* temp_ident = new string();
					ret_type temp_ret;

					next_symbol();
					temp_ret = expression(temp_ident);
					if (current_token->t_type != RIGHT_PAR)
						ERROR(-1);
					else {
						next_symbol();
						
						switch (temp_ret) {
						case INTRET:
							new_add_mid_code(PRINTFOP, new string("\\n"), new string("int"), temp_ident);
							break;
						case CHARRET:
							new_add_mid_code(PRINTFOP, new string("\\n"), new string("char"), temp_ident);
							break;
						}

						//cout << temp_line << " \t" << temp_pos << " :\t";
						//cout << "This is Printf String Expression." << endl;
					}
				}
				else { // current_token ->t_type == RIGHT_PAR
					next_symbol();
					//cout << "This is Printf String." << endl;
				}
			}
			else {
				string* temp_ident = new string();
				ret_type temp_ret;

				//next_symbol(); ??????????????????????????????????????这个一直是错的？？？2018.12.06_14:47
				temp_ret = expression(temp_ident);


				if (current_token->t_type != RIGHT_PAR)
					ERROR(-1);
				else {
					next_symbol();

					switch (temp_ret) {
					case INTRET:
						new_add_mid_code(PRINTFOP, new string("\\n"), new string("int"), temp_ident);
						break;
					case CHARRET:
						new_add_mid_code(PRINTFOP, new string("\\n"), new string("char"), temp_ident);
						break;
					}

					//cout << temp_line << " \t" << temp_pos << " :\t";
					//cout << "This is Printf Expression." << endl;
				}
			}
		}
	}


}

void C0_Compiler::ret_state(string* func_name) {
	ret_type return_ret_type;

	if (current_token->t_type != RETURN_SYMBOL) {
		ERROR(-1);
	}
	else {
		next_symbol();
		if (current_token->t_type != LEFT_PAR) //return后无表达式
			return_ret_type = VOIDRET;
		else {
			next_symbol();

			string* temp_ident = new string();

			return_ret_type = expression(temp_ident);

			if (var_is_temp(temp_ident)) { //如果是临时变量，换#RET
				//BUG!!!!又同时修改了两个！！！！
				mid_code* temp_code;
				//mid_code* temp_code = mid_code_table[mid_code_table_top - 1];????访问出错???
				//temp_code = new_mid_code(ADDOP, new string(), new string(), new string()); 
				temp_code = get_mid_code(mid_code_table_top - 1);
				
				*temp_code->des = string("#RET");
			}
			else {
				new_add_mid_code(ADDOP, temp_ident, ZERO_VAR, new string("#RET"));
			}

			if (current_token->t_type != RIGHT_PAR)
				ERROR(-1);
			else {
				next_symbol();
			}
		}
	}
	//? main ?
	//函数结尾标为@end
	string* func_end = new string();
	func_end->append(func_name->c_str());
	func_end->append("_end");

	if (*func_name == "main") {
		new_add_mid_code(EXITOP, new string(), new string(), new string());
	}
	else {
		new_add_mid_code(GOTOOP, new string(), new string(), func_end);
	}


	this->return_state_list[this->return_state_number] = return_ret_type;
	this->return_state_number++;
	
	//cout << "This is Return Statement." << endl;
}
