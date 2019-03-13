#include "C0_Compiler.h"

//cout << token_type_enum_to_string[current_token->t_type] << " \t" << current_token->line << " \t" << current_token->pos << endl;

void C0_Compiler::grammar_analysis() {
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
			if (current_token->t_type == COMMA || current_token->t_type == SEMICOLON) {
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
				back_spot();
				ret_func_def();
			}
			else {
				back_spot();
				non_ret_func_def();
			}

		}
		else {
			back_spot();
			main_def();
		}
	}

}

void C0_Compiler::type_ident() {
	if (current_token->t_type != INT_SYMBOL && current_token->t_type != CHAR_SYMBOL)
		ERROR(-1);
	else {
		next_symbol();
	}
}

void C0_Compiler::ident() {
	if (current_token->t_type != IDENT)
		ERROR(-1);
	else {
		next_symbol();
	}
}

void C0_Compiler::signed_int() {
	if (current_token->t_type == PLUS || current_token->t_type == SUB)
		next_symbol();
	if (current_token->t_type == INT) {
		//cout << "This is Signed Int." << endl;
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
	cout << temp_line << " \t" << temp_pos << " :\t";
	cout << "This is Const Declare. Const Definition is Over." << endl;
}

void C0_Compiler::const_def() {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	if (current_token->t_type != INT_SYMBOL && current_token->t_type != CHAR_SYMBOL) {
		ERROR(-1);
	}
	else if (current_token->t_type == INT_SYMBOL) {
		do {
			next_symbol();
			if (current_token->t_type != IDENT)
				ERROR(-1);
			else {
				next_symbol();
				if (current_token->t_type != BECOME)
					ERROR(-1);
				else {
					next_symbol();
					signed_int();
				}
			}
		} while (current_token->t_type == COMMA);
	}
	else {// current_token->t_type == CHAR_SYMBOL
		do {
			next_symbol();
			if (current_token->t_type != IDENT)
				ERROR(-1);
			else {
				next_symbol();
				if (current_token->t_type != BECOME)
					ERROR(-1);
				else {
					next_symbol();
					if (current_token->t_type != CHAR)
						ERROR(-1);
					else {
						next_symbol();
					}
				}
			}
		} while (current_token->t_type == COMMA);
	}
	cout << temp_line << " \t" << temp_pos << " :\t";
	cout << "This is Const Definition." << endl;
}

void C0_Compiler::declare_head() {
	if (current_token->t_type == INT_SYMBOL) {
		next_symbol();
		if (current_token->t_type == IDENT)
			cout << "This is Declare Head." << endl;
		else {
			ERROR(-1);
		}
		next_symbol();
	}
	else if (current_token->t_type == CHAR_SYMBOL) {
		next_symbol();
		if (current_token->t_type == IDENT)
			cout << "this is Declare Head." << endl;
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

			/////
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
	cout << temp_line << " \t" << temp_pos << " :\t";
	cout << "This is Var Declare. Var Definition is Over." << endl;
}

void C0_Compiler::var_def() {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	if (current_token->t_type == INT_SYMBOL || current_token->t_type == CHAR_SYMBOL) {
		do {
			next_symbol();

			if (current_token->t_type == IDENT) {
				next_symbol();

				if (current_token->t_type == LEFT_BRACKET) {

					next_symbol();
					if (current_token->t_type == INT) {
						//大于0判断
						next_symbol();
						if (current_token->t_type == RIGHT_BRACKET) {
							next_symbol();
						}
						else
							ERROR(-1);
					}
					else
						ERROR(-1);
				}
				else {
					;
				}
			}
			else
				ERROR(-1);
		} while (current_token->t_type == COMMA);
		cout << temp_line << " \t" << temp_pos << " :\t";
		cout << "This is Var Def." << endl;
	}
	else
		ERROR(-1);
}

void C0_Compiler::ret_func_def() {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	if (current_token->t_type != INT_SYMBOL && current_token->t_type != CHAR_SYMBOL)
		ERROR(-1);
	else {
		next_symbol();
		if (current_token->t_type != IDENT)
			ERROR(-1);
		else {
			next_symbol();

			if (current_token->t_type == LEFT_PAR) {
				next_symbol();

				para_list();
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
				complex_state();

				if (current_token->t_type != RIGHT_BRACE)
					ERROR(-1);
				else {
					next_symbol();
					cout << temp_line << " \t" << temp_pos << " :\t";
					cout << "This is Return Function Definition. Over." << endl;
				}
			}
		}
	}
}

void C0_Compiler::non_ret_func_def() {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	if (current_token->t_type != VOID_SYMBOL)
		ERROR(-1);
	else {
		next_symbol();
		if (current_token->t_type != IDENT)
			ERROR(-1);
		else {
			next_symbol();
			if (current_token->t_type != LEFT_BRACE && current_token->t_type != LEFT_PAR)
				ERROR(-1);
			else if (current_token->t_type == LEFT_PAR) {
				next_symbol();
				para_list();
				if (current_token->t_type != RIGHT_PAR)
					ERROR(-1);
				else {
					next_symbol();
					if (current_token->t_type != LEFT_BRACE)
						ERROR(-1);
					else {
						next_symbol();
						complex_state();
						if (current_token->t_type != RIGHT_BRACE)
							ERROR(-1);
						else {
							next_symbol();
							cout << temp_line << " \t" << temp_pos << " :\t";
							cout << "This is Return Function Definition. Over" << endl;
						}
					}
				}
			}
			else { // current_token ->t_type == LEFT_BRACE
				next_symbol();
				complex_state();
				if (current_token->t_type != RIGHT_BRACE)
					ERROR(-1);
				else {
					next_symbol();
					cout << temp_line << " \t" << temp_pos << " :\t";
					cout << "This is Return Function Definition. Over." << endl;
				}
			}
		}
	}
}

void C0_Compiler::complex_state() {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	if (current_token->t_type == CONST_SYMBOL) {
		const_decl();
	}

	if (current_token->t_type == INT_SYMBOL || current_token->t_type == CHAR_SYMBOL) {
		var_decl();
	}
	statement_list();
	cout << temp_line << " \t" << temp_pos << " :\t";
	cout << "This is Complex Statements. Over." << endl;
}

void C0_Compiler::para_list() {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	type_ident();
	ident();

	while (current_token->t_type == COMMA) {
		next_symbol();
		type_ident();
		ident();
	}
	cout << temp_line << " \t" << temp_pos << " :\t";
	cout << "This is Para List." << endl;
}

void C0_Compiler::main_def() {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

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
						complex_state();
						if (current_token->t_type != RIGHT_BRACE)
							ERROR(-1);
						else {
							//next_symbol(); ???????你是傻屌嘛
							cout << temp_line << " \t" << temp_pos << " :\t";
							cout << "This is Main Definition. Over." << endl;
						}
					}
				}
			}
		}
	}
}

void C0_Compiler::expression() {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;


	if (current_token->t_type == PLUS) {
		next_symbol();
	}
	else if (current_token->t_type == SUB) {
		next_symbol();
	}
	term();
	while (current_token->t_type == PLUS || current_token->t_type == SUB) {
		next_symbol();
		term();
	}
	cout << temp_line << " \t" << temp_pos << " :\t";
	cout << "This is An Expression." << endl;
}

void C0_Compiler::term() {
	factor();
	while (current_token->t_type == MULT || current_token->t_type == DIV) {
		next_symbol();
		factor();
	}
}

void C0_Compiler::factor() {
	if (current_token->t_type == IDENT) {
		// !!!!!!!!!
		//if(current_token) //判断是函数还是变量常量*************************************
		//    ;
		// !!!!!!!!!
		save_spot();
		next_symbol();
		if (current_token->t_type == LEFT_PAR) {
			back_spot();
			ret_func_call();
		}
		else if (current_token->t_type == LEFT_BRACKET) {
			next_symbol();
			expression();
			if (current_token->t_type != RIGHT_BRACKET)
				ERROR(-1);
			else
				next_symbol();
		}
		else {
			back_spot();
			ident();
		}
	}
	else if (current_token->t_type == INT)
		next_symbol();
	else if (current_token->t_type == CHAR)
		next_symbol();
	else if (current_token->t_type == LEFT_PAR) {
		next_symbol();
		expression();
		if (current_token->t_type != RIGHT_PAR) {
			ERROR(-1);
		}
		else
			next_symbol();
	}
}

void C0_Compiler::statement() {
	if (current_token->t_type == IF_SYMBOL)
		if_state();
	else if (current_token->t_type == DO_SYMBOL)
		dowhile_state();
	else if (current_token->t_type == SWITCH_SYMBOL)
		switch_state();
	else if (current_token->t_type == IDENT) {
		//需要重写
		save_spot();
		next_symbol();
		if (current_token->t_type == BECOME || current_token->t_type == LEFT_BRACKET) {
			back_spot();
			become_state();
		}
		else if (current_token->t_type == LEFT_PAR) {
			back_spot();
			ret_func_call();
		}
		else {
			back_spot();
			non_ret_func_call();
		}
		if (current_token->t_type != SEMICOLON)
			ERROR(-1);
		else {
			next_symbol();
		}

		//next_symbol();//赋值语句，此处需要回溯！！！！！！！！！！！！！！！！！！！！！！！！！！
		//if()
		// 如果是返回值函数
		//ret_func_call();
		// 如果是无返回值函数
		//non_ret_func_call();
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

	else if (current_token->t_type == SEMICOLON)
		next_symbol();
	else if (current_token->t_type == RETURN_SYMBOL) {
		ret_state();

		if (current_token->t_type != SEMICOLON) {
			ERROR(-1);
		}
		else
			next_symbol();
	}
	else if (current_token->t_type == LEFT_BRACE) {
		next_symbol();
		statement_list();
		if (current_token->t_type != RIGHT_BRACE)
			ERROR(-1);
		else {
			next_symbol();
		}
	}

	else
		ERROR(-1);
}//未完成！可能有错！

void C0_Compiler::become_state() {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	if (current_token->t_type != IDENT)
		ERROR(-1);
	else {
		next_symbol();
		if (current_token->t_type == BECOME) {
			next_symbol();
			expression();
		}
		else if (current_token->t_type == LEFT_BRACKET) {

			next_symbol();

			expression();
			if (current_token->t_type != RIGHT_BRACKET)
				ERROR(-1);
			else {
				next_symbol();
				if (current_token->t_type != BECOME)
					ERROR(-1);
				else {
					next_symbol();
					expression();
				}

			}
		}
		else
			ERROR(-1);
	}
	cout << temp_line << " \t" << temp_pos << " :\t";
	cout << "This is Become Statement." << endl;
}

void C0_Compiler::if_state() {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	if (current_token->t_type != IF_SYMBOL)
		ERROR(-1);
	else {
		next_symbol();
		if (current_token->t_type != LEFT_PAR)
			ERROR(-1);
		else {
			next_symbol();
			condition();
			if (current_token->t_type != RIGHT_PAR)
				ERROR(-1);
			else {
				next_symbol();
				statement();
				if (current_token->t_type != ELSE_SYMBOL)
					ERROR(-1);
				else {
					next_symbol();
					statement();
				}
			}
		}
	}
	cout << temp_line << " \t" << temp_pos << " :\t";
	cout << "This is If Statement." << endl;
}

void C0_Compiler::condition() {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	expression();
	if (current_token->t_type != GTR && current_token->t_type != GTREQL
		&& current_token->t_type != LSS && current_token->t_type != LSSEQL
		&& current_token->t_type != EQLEQL && current_token->t_type != NEQ)
		;
	else {
		next_symbol();
		expression();
	}
	cout << temp_line << " \t" << temp_pos << " :\t";
	cout << "This is Condition." << endl;
}

void C0_Compiler::dowhile_state() {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	if (current_token->t_type != DO_SYMBOL)
		ERROR(-1);
	else {
		next_symbol();
		statement();
		if (current_token->t_type != WHILE_SYMBOL)
			ERROR(-1);
		else {
			next_symbol();
			if (current_token->t_type != LEFT_PAR)
				ERROR(-1);
			else {
				next_symbol();
				condition();
				if (current_token->t_type != RIGHT_PAR)
					ERROR(-1);
				else {
					next_symbol();
				}
			}
		}
	}
	cout << temp_line << " \t" << temp_pos << " :\t";
	cout << "This is Do While Statement." << endl;
}

void C0_Compiler::const_value() {
	if (current_token->t_type != INT && current_token->t_type != CHAR)
		ERROR(-1);
	else {
		;//////////////////////////////////////////
		next_symbol();
	}
}

void C0_Compiler::switch_state() {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	if (current_token->t_type != SWITCH_SYMBOL)
		ERROR(-1);
	else {
		next_symbol();
		if (current_token->t_type != LEFT_PAR)
			ERROR(-1);
		else {
			next_symbol();
			expression();
			if (current_token->t_type != RIGHT_PAR)
				ERROR(-1);
			else {
				next_symbol();
				if (current_token->t_type != LEFT_BRACE)
					ERROR(-1);
				else {
					next_symbol();
					case_list();
					default_state();
					if (current_token->t_type != RIGHT_BRACE)
						ERROR(-1);
					else {
						next_symbol();
					}
				}
			}
		}
	}
	cout << temp_line << " \t" << temp_pos << " :\t";
	cout << "This is Switch Statement." << endl;
}

void C0_Compiler::case_list() {
	do {
		case_state();
	} while (current_token->t_type == CASE_SYMBOL);
}

void C0_Compiler::case_state() {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	if (current_token->t_type != CASE_SYMBOL)
		ERROR(-1);
	else {
		next_symbol();
		const_value();
		if (current_token->t_type != COLON)
			ERROR(-1);
		else {
			next_symbol();
			statement();
		}
	}
	cout << temp_line << " \t" << temp_pos << " :\t";
	cout << "This is Case State." << endl;
}

void C0_Compiler::default_state() {
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
			statement();
		}
	}
	cout << temp_line << " \t" << temp_pos << " :\t";
	cout << "This is Default Statement." << endl;
}

void C0_Compiler::ret_func_call() {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	ident();
	if (current_token->t_type != LEFT_PAR)
		;
	else {
		next_symbol();
		val_para_list();
		if (current_token->t_type != RIGHT_PAR)
			ERROR(-1);
		else {
			next_symbol();
		}
	}
	cout << temp_line << " \t" << temp_pos << " :\t";
	cout << "This is Return Function Call." << endl;
}

void C0_Compiler::non_ret_func_call() {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	ident();
	if (current_token->t_type != LEFT_PAR)
		;
	else {
		next_symbol();
		val_para_list();
		if (current_token->t_type != RIGHT_PAR)
			ERROR(-1);
		else {
			next_symbol();
		}
	}
	cout << temp_line << " \t" << temp_pos << " :\t";
	cout << "This is Non Return Function Call." << endl;
}

void C0_Compiler::val_para_list() {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	expression();
	while (current_token->t_type == COMMA) {
		next_symbol();
		expression();
	}
	cout << temp_line << " \t" << temp_pos << " :\t";
	cout << "This is Val Para List." << endl;
}

void C0_Compiler::statement_list() {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	while (current_token->t_type != RIGHT_BRACE) {
		statement();
	}
	cout << temp_line << " \t" << temp_pos << " :\t";
	cout << "This is Statement List. Over." << endl;
}

void C0_Compiler::scanf_state() {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	if (current_token->t_type != SCANF_SYMBOL)
		ERROR(-1);
	else {
		next_symbol();
		if (current_token->t_type != LEFT_PAR)
			ERROR(-1);
		else {
			next_symbol();
			ident();
			while (current_token->t_type == COMMA) {
				next_symbol();
				ident();
			}

			if (current_token->t_type != RIGHT_PAR)
				ERROR(-1);
			else {
				next_symbol();
			}
		}
	}
	cout << temp_line << " \t" << temp_pos << " :\t";
	cout << "This is Scanf Statement." << endl;
}

void C0_Compiler::printf_state() {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	if (current_token->t_type != PRINTF_SYMBOL)
		ERROR(-1);
	else {
		next_symbol();
		if (current_token->t_type != LEFT_PAR)
			ERROR(-1);
		else {
			next_symbol();
			if (current_token->t_type == STRING) {
				next_symbol();
				if (current_token->t_type != COMMA && current_token->t_type != RIGHT_PAR)
					ERROR(-1);
				else if (current_token->t_type == COMMA) {
					next_symbol();
					expression();
					if (current_token->t_type != RIGHT_PAR)
						ERROR(-1);
					else {
						next_symbol();
						cout << temp_line << " \t" << temp_pos << " :\t";
						cout << "This is Printf String Expression." << endl;
					}
				}
				else { // current_token ->t_type == RIGHT_PAR
					next_symbol();
					cout << temp_line << " \t" << temp_pos << " :\t";
					cout << "This is Printf String." << endl;
				}
			}
			else {
				expression();
				if (current_token->t_type != RIGHT_PAR)
					ERROR(-1);
				else {
					next_symbol();
					cout << temp_line << " \t" << temp_pos << " :\t";
					cout << "This is Printf Expression." << endl;
				}
			}
		}
	}
}

void C0_Compiler::ret_state() {
	int temp_line, temp_pos;
	temp_line = current_token->line;
	temp_pos = current_token->pos;

	if (current_token->t_type != RETURN_SYMBOL) {
		ERROR(-1);
	}
	else {
		next_symbol();
		if (current_token->t_type != LEFT_PAR)
			;
		else {
			next_symbol();
			expression();

			if (current_token->t_type != RIGHT_PAR)
				ERROR(-1);
			else {
				next_symbol();
			}
		}
	}
	cout << temp_line << " \t" << temp_pos << " :\t";
	cout << "This is Return Statement." << endl;
}
