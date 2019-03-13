#include "C0_Compiler.h"

void C0_Compiler::generate()
{
	this->init_ascii();
	this->generate_mips(new std::string(".text"));

	this->generate_mips(ADD, FP, SP, R0);
	this->generate_mips(ADDI, GP, GP, 0x10000);
	this->handle_goto(new std::string("main"));
	for (int i = 0; i < mid_code_table_top; i++)
	{
		mid_code *code = mid_code_table[i];
		switch (code->op)
		{
		case FUNCBEGINOP:
			this->currentRef += 1;
			this->func_begin(code->des);
			break;
		case EQLEQLOP:
		case NEQOP:
		case GTROP:
		case GTREQLOP:
		case LSSOP:
		case LSSEQLOP:
			this->handle_branch(code);
			break;
		case GOTOOP:
			this->handle_goto(code->des);
			break;
		case LABELOP:
			this->gen_mips_label(code->des);
			break;
		case PARAVALOP:
			this->handle_para_val(code->des);
			break;
		case ARRAYTOOP:
			this->handle_arrayto(code);
			break;
		case CALLOP:
			this->handle_call(code->des);
			break;
		case RETOP:
			this->handle_ret(code->des);
			break;
		case DIVOP:
		case MULTOP:
			this->handle_mult_div(code);
			break;
		case ADDOP:
			this->handle_add(code);
			break;
		case SUBOP:
			this->handle_sub(code);
			break;
		case TOARRAYOP:
			this->handle_toarray(code);
			break;
		case SCANFOP:
			this->handle_scanf(code);
			break;
		case PRINTFOP:
			this->handle_printf(code);
			break;
		case EXITOP:
			this->handle_exit();
			break;
		}
	}
}

void C0_Compiler::push_mips(std::string *order)
{

	std::string *save = new std::string();
	*save = *order;
	this->mipsCodes[this->mipsIndex++] = save;
}

void C0_Compiler::write_mips_to_file()
{
	for (int i = 0; i < this->mipsIndex; i++)
	{
		objectcodefile << *(this->mipsCodes[i]) << endl;
	}
}


void C0_Compiler::object_init()
{
	this->mipsIndex = 0;
	this->currentRef = -1;
	this->reg_init();
}

void C0_Compiler::reg_init()
{
	for (int i = 0; i < 30; i++)
	{
		all_reg[i] = 0;
	}
}

void C0_Compiler::init_ascii()
{
	this->generate_mips(new std::string(".data"));

	for (int i = 0; i < this->string_number; i++)
	{
		std::string *str = string_table[i];
		std::stringstream ss = std::stringstream();
		ss << "$Message" << i << ":" << ".asciiz" << "\"" << *str << "\"";
		this->generate_mips(&(ss.str()));
	}
}

void C0_Compiler::generate_mips(std::string *order, std::string *rs, int num)
{
	std::stringstream ss = std::stringstream();
	ss << "\t" << *order << "\t" << *rs << "\t,\t" << num << std::endl;
	this->push_mips(&ss.str());
}

void C0_Compiler::generate_mips(std::string *order, std::string *rs, int num, std::string *rt)
{
	std::stringstream ss = std::stringstream();
	ss << "\t" << *order << "\t" << *rs << "\t,\t" << num << "(" << *rt << ")" << std::endl;
	this->push_mips(&ss.str());
}

void C0_Compiler::generate_mips(std::string *order, std::string *rd, std::string *rt, int imme)
{
	std::stringstream ss = std::stringstream();
	ss << "\t" << *order << "\t" << *rd << "\t,\t" << *rt << "\t,\t" << imme << std::endl;
	this->push_mips(&ss.str());
}

void C0_Compiler::generate_mips(std::string *order, std::string *rd, std::string *rs, std::string *rt)
{
	std::stringstream ss = std::stringstream();
	ss << "\t" << *order << "\t" << *rd << "\t,\t" << *rs << "\t,\t" << *rt << std::endl;
	this->push_mips(&ss.str());
}

void C0_Compiler::generate_mips(std::string *order, std::string *rs, std::string *label)
{
	std::stringstream ss = std::stringstream();
	ss << "\t" << *order << "\t" << *rs << "\t,\t" << *label << std::endl;
	this->push_mips(&ss.str());
}

void C0_Compiler::generate_mips(std::string *order, std::string *target)
{
	std::stringstream ss = std::stringstream();
	ss << "\t" << *order << "\t" << *target << std::endl;
	this->push_mips(&ss.str());
}

void C0_Compiler::generate_mips(std::string *order)
{
	std::stringstream ss = std::stringstream();
	ss << "\t" << *order << std::endl;
	this->push_mips(&ss.str());
}

void C0_Compiler::gen_mips_label(std::string *label)
{
	std::string *newlab = new std::string();

	*newlab = *newlab + ":\n";
	this->push_mips(newlab);
	delete newlab;
}

void C0_Compiler::func_begin(std::string *name)
{

	symbol *sym = 0;
	this->find_symbol(name, current_func, false);

	this->gen_mips_label(sym->name);


	int currentaddress = 0;
	int paranum = sym->number;
	if (paranum != 0)
		this->generate_mips(ADDI, SP, SP, paranum * 4);



	for (int i = 0; i < 30; i++)
	{
		std::stringstream ss = std::stringstream();
		int index = i % (30 / 2);
		//char *sym = "$t";
		ss << "$t" << index;
		this->generate_mips(SW, &(ss.str()), -currentaddress * 4, SP);
		currentaddress += 1;
	}


	this->generate_mips(SW, FP, -currentaddress * 4, SP);

	currentaddress += 1;
	this->generate_mips(SW, RA, -currentaddress * 4, SP);
	currentaddress += 1;

	this->generate_mips(ADD, FP, SP, R0);

	this->generate_mips(ADDI, SP, SP, -currentaddress * 4);

}

void C0_Compiler::handle_ret(std::string *name)
{

	symbol *sym = 0;
	this->find_symbol(name, current_func, false);


	int currentaddress = 1;
	this->generate_mips(LW, RA, currentaddress * 4, SP);

	currentaddress += 1;
	this->generate_mips(LW, FP, currentaddress * 4, SP);
	currentaddress += 1;

	this->reg_init();

	for (int i = 30 - 1; i >= 0; i--)
	{
		std::stringstream ss = std::stringstream();
		int index = i % (30 / 2);
		//char *sym = new string("$s")->c_str;
		ss << "$s" << index;
		this->generate_mips(LW, &(ss.str()), currentaddress * 4, SP);
		currentaddress += 1;
	}


	currentaddress += -1;
	this->generate_mips(ADDI, SP, SP, currentaddress * 4);

	this->generate_mips(JR, RA);
	this->generate_mips(NOP);
}



void C0_Compiler::find_symbol(std::string *name, symbol **resultsym, bool *global)
{
	symbol **symtab = func_sym_table[this->currentRef];
	int length = func_sym_number[this->currentRef];
	for (int i = 0; i < length; i++)
	{
		symbol *sym = symtab[i];

		if (*name == *(sym->name))
		{
			*resultsym = sym;
			*global = false;
			return;
		}
	}
	for (int i = 0; i < sym_table_top; i++)
	{
		symbol *sym = sym_table[i];
		if (*name == *(sym->name))
		{
			*resultsym = sym;
			*global = true;
			return;
		}
	}

}



void C0_Compiler::get_use_reg(std::string *rs, std::string *reg)
{


	if (this->var_is_ret(rs))
	{
		*reg = *V0;
	}
	else if (this->var_is_number(rs))
	{

		if ((*rs)[0] == '0')
		{
			*reg = *R0;
		}
		else {
			this->generate_mips(LI, reg, rs);
		}
	}
	else {

		bool global = false;
		symbol *sym = 0;
		this->find_symbol(rs, &sym, &global);
		int regindex = sym->reg_index;
		if (regindex == -1)
		{

			int offset = sym->address;
			if (global)
			{
				this->generate_mips(LW, reg, 4 * offset, GP);
			}
			else {
				this->generate_mips(LW, reg, -offset * 4, FP);
			}
		}
		else {

			std::stringstream ss = std::stringstream();
			ss << (regindex < (30 / 2) ? "$t" : "$s") << regindex % (30 / 2);
			*reg = ss.str();

			if (sym->i_type == PARAIDENT && (!all_reg[regindex] || !(*(all_reg[regindex]) == *sym->name)))
			{
				this->generate_mips(LW, reg, -sym->address * 4, FP);
			}
			all_reg[regindex] = sym->name;
		}
	}
}

void C0_Compiler::get_result_reg(std::string *rd, std::string *reg) {


	if (this->var_is_ret(rd))
	{

		*reg = *V0;
	}
	else {

		bool global = false;
		symbol *sym = 0;
		this->find_symbol(rd, &sym, &global);
		int regindex = sym->reg_index;
		if (regindex == -1)
		{


		}
		else {

			std::stringstream ss = std::stringstream();
			ss << (regindex < (30 / 2) ? "$t" : "$s") << regindex % (30 / 2);
			*reg = ss.str();
			all_reg[regindex] = sym->name;
		}
	}

}


void C0_Compiler::writeBack(std::string *rd, std::string *reg)
{
	if (!this->var_is_ret(rd))
	{
		symbol *sym = 0;
		bool global = false;
		this->find_symbol(rd, &sym, &global);
		if (sym && sym->reg_index == -1)
		{

			if (global)
			{
				this->generate_mips(SW, reg, 4 * sym->address, GP);
			}
			else {
				this->generate_mips(SW, reg, -sym->address * 4, FP);
			}
		}
	}
}

void C0_Compiler::handle_branch(mid_code *code)
{
	std::string *op1 = code->var_1;
	std::string *op2 = code->var_2;

	std::string *reg1 = new std::string();
	*reg1 = *T9;
	std::string *reg2 = new std::string();
	*reg2 = *T8;
	std::string *rstreg = new std::string();
	*rstreg = *T7;

	if (code->op == EQLEQLOP || code->op == NEQOP)
	{
		if (this->var_is_number(op1))
		{
			if (this->var_is_number(op2))
			{

				int value1 = atoi(op1->c_str());
				int value2 = atoi(op2->c_str());
				value2 = value1 - value2;
				std::string constvalue = std::string();
				constvalue = to_string(value2);
				this->get_use_reg(&constvalue, reg1);
				*reg2 = *R0;
			}
			else {

				this->get_use_reg(op1, reg1);
				this->get_use_reg(op2, reg2);
			}
		}
		else {
			this->get_use_reg(op1, reg1);
			this->get_use_reg(op2, reg2);
		}

		this->generate_mips(code->op == EQLEQLOP ? BEQ : BNE, reg1, reg2, code->des);
	}
	else {
		if (!this->var_is_number(op1))
		{


			this->get_use_reg(op1, reg1);
			if (this->var_is_number(op2))
			{

				this->generate_mips(SUBI, rstreg, reg1, op2);
			}
			else {

				this->get_use_reg(op2, reg2);

				this->generate_mips(SUBST, rstreg, reg1, reg2);
			}
		}
		else {

			if (this->var_is_number(op2))
			{

				int value1 = atoi(op1->c_str());
				int value2 = atoi(op2->c_str());
				value2 = value1 - value2;
				std::string constvalue = std::string();
				constvalue = to_string(value2);
				this->get_use_reg(&constvalue, rstreg);
			}
			else {

				this->get_use_reg(op2, reg2);
				this->get_use_reg(op1, reg1);
				this->generate_mips(SUBST, rstreg, reg1, reg2);

			}
		}
		switch (code->op)
		{
		case GTREQLOP:
			this->generate_mips(BGEZ, rstreg, code->des);
			break;
		case GTROP:
			this->generate_mips(BGTZ, rstreg, code->des);
			break;
		case LSSEQLOP:
			this->generate_mips(BLEZ, rstreg, code->des);
			break;
		case LSSOP:
			this->generate_mips(BLTZ, rstreg, code->des);
			break;
		}
	}

	this->generate_mips(NOP);

	delete rstreg;
	delete reg1;
	delete reg2;
}


void C0_Compiler::handle_goto(std::string *label)
{
	std::string *newlab = new std::string();

	this->generate_mips(J, newlab);
	this->generate_mips(NOP);
}

void C0_Compiler::handle_para_val(std::string *para)
{

	std::string *rstreg = new std::string();
	*rstreg = *T7;
	this->get_use_reg(para, rstreg);
	this->generate_mips(SW, rstreg, 0, SP);
	this->generate_mips(ADDI, SP, SP, -4);
	delete rstreg;
}

void C0_Compiler::handle_call(std::string *name)
{
	std::string *newlab = new std::string();

	this->generate_mips(JAL, newlab);
	this->generate_mips(NOP);
	delete newlab;
}

void C0_Compiler::handle_add(mid_code *code)
{
	std::string *op1 = code->var_1;
	std::string *op2 = code->var_2;
	std::string *rst = code->des;

	std::string *reg1 = new std::string();
	*reg1 = *T9;
	std::string *reg2 = new std::string();
	*reg2 = *T8;
	std::string *rstreg = new std::string();
	*rstreg = *T7;
	if (this->var_is_number(op1))
	{
		if (!this->var_is_number(op2))
		{

			this->get_use_reg(op2, reg2);
			this->get_result_reg(rst, rstreg);
			this->generate_mips(ADDI, rstreg, reg2, op1);
		}
		else {

			int value1 = atoi(op1->c_str());
			int value2 = atoi(op2->c_str());
			value2 = value1 + value2;
			this->get_result_reg(rst, rstreg);
			this->generate_mips(LI, rstreg, value2);
		}
	}
	else {

		this->get_use_reg(op1, reg1);
		if (this->var_is_number(op2))
		{

			this->get_result_reg(rst, rstreg);

			this->generate_mips(ADDI, rstreg, reg1, op2);

		}
		else {

			this->get_use_reg(op2, reg2);
			this->get_result_reg(rst, rstreg);
			this->generate_mips(ADD, rstreg, reg1, reg2);
		}
	}
	this->writeBack(rst, rstreg);

	delete reg1;
	delete reg2;
	delete rstreg;
}

void C0_Compiler::handle_sub(mid_code *code)
{
	std::string *op1 = code->var_1;
	std::string *op2 = code->var_2;
	std::string *rst = code->des;

	std::string *reg1 = new std::string();
	*reg1 = *T9;
	std::string *reg2 = new std::string();
	*reg2 = *T8;
	std::string *rstreg = new std::string();
	*rstreg = *T7;
	if (this->var_is_number(op1))
	{
		this->get_use_reg(op1, reg1);
		this->get_use_reg(op2, reg2);
		this->get_result_reg(rst, rstreg);
		this->generate_mips(SUBST, rstreg, reg1, reg2);
	}
	else {

		this->get_use_reg(op1, reg1);
		if (!this->var_is_number(op2))
		{
			this->get_use_reg(op2, reg2);


			this->get_result_reg(rst, rstreg);


			this->generate_mips(SUBST, rstreg, reg1, reg2);
		}
		else {

			this->get_result_reg(rst, rstreg);
			this->generate_mips(SUBI, rstreg, reg1, op2);
		}
	}
	this->writeBack(rst, rstreg);
	delete reg1;
	delete reg2;
	delete rstreg;
}


void C0_Compiler::handle_mult_div(mid_code *code)
{
	std::string *op1 = code->var_1;
	std::string *op2 = code->var_2;
	std::string *rst = code->des;

	std::string *reg1 = new std::string();
	*reg1 = *T9;
	std::string *reg2 = new std::string();
	*reg2 = *T8;
	std::string *rstreg = new std::string();
	*rstreg = *T7;
	this->get_use_reg(op1, reg1);
	this->get_use_reg(op2, reg2);
	this->get_result_reg(rst, rstreg);
	if (code->op == MULTOP)
	{
		this->generate_mips(MULOBJ, rstreg, reg1, reg2);
	}
	else {
		this->generate_mips(DIVOBJ, reg1, reg2);
		this->generate_mips(MFLO, rstreg);
	}
	this->writeBack(rst, rstreg);
	delete reg1;
	delete reg2;
	delete rstreg;
}

void C0_Compiler::handle_arrayto(mid_code *code)
{
	std::string *reg2 = new std::string();
	*reg2 = *T8;
	std::string *rstreg = new std::string();
	*rstreg = *T7;

	bool global = false;
	symbol *sym = 0;
	this->find_symbol(code->var_1, &sym, &global);
	this->get_use_reg(code->var_2, reg2);
	this->generate_mips(ADDI, T9, reg2, sym->address);
	this->generate_mips(SLL, T9, T9, 2);
	this->get_result_reg(code->des, rstreg);
	if (global)
	{
		this->generate_mips(ADDU, T9, T9, GP);
		this->generate_mips(LW, rstreg, 0, T9);
	}
	else {
		this->generate_mips(SUBST, T9, FP, T9);
		this->generate_mips(LW, rstreg, 0, T9);
	}
	this->writeBack(code->des, rstreg);


	delete reg2;
	delete rstreg;
}


void C0_Compiler::handle_toarray(mid_code *code)
{
	std::string *rs = code->var_1;
	std::string *index = code->var_2;
	std::string *name = code->des;

	std::string *reg1 = new std::string();
	*reg1 = *T9;
	std::string *reg2 = new std::string();
	*reg2 = *T8;

	bool global = false;
	symbol *sym = 0;
	this->find_symbol(name, &sym, &global);

	this->get_use_reg(index, reg2);
	this->generate_mips(ADDI, T7, reg2, sym->address);
	this->generate_mips(SLL, T7, T7, 2);
	this->get_use_reg(rs, reg1);
	if (global)
	{
		this->generate_mips(ADDU, T7, T7, GP);
		this->generate_mips(SW, reg1, 0, T7);
	}
	else {
		this->generate_mips(SUBST, T7, FP, T7);
		this->generate_mips(SW, reg1, 0, T7);
	}
	delete reg1;
	delete reg2;
}

void C0_Compiler::handle_scanf(mid_code *code)
{
	if ((*code->var_2)[0] == 'i')
	{

		this->generate_mips(LI, V0, 5);
	}
	else {

		this->generate_mips(LI, V0, 12);
	}
	this->generate_mips(SYSCALL);

	std::string *rstreg = new std::string();
	*rstreg = *T7;
	this->get_result_reg(code->des, rstreg);
	this->generate_mips(ADD, rstreg, V0, R0);
	this->writeBack(code->des, rstreg);

	delete rstreg;
}

void C0_Compiler::handle_printf(mid_code *code)
{

	if (*code->var_2 == std::string("0"))
	{

		this->generate_mips(LI, V0, 4);
		std::stringstream ss = std::stringstream();
		ss << "$Message" << *(code->des);
		this->generate_mips(LA, A0, &(ss.str()));
		this->generate_mips(SYSCALL);
	}
	else {

		this->generate_mips(LI, V0, (*code->var_2)[0] == 'i' ? 1 : 11);

		std::string *rstreg = new std::string();
		*rstreg = *T7;
		this->get_use_reg(code->des, rstreg);
		this->generate_mips(ADD, A0, rstreg, R0);
		this->generate_mips(SYSCALL);
		delete rstreg;
	}
}

void C0_Compiler::handle_exit()
{
	this->generate_mips(LI, V0, 10);
	this->generate_mips(SYSCALL);
}

