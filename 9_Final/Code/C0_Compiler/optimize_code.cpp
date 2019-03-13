#include "C0_Compiler.h"

void C0_Compiler::use_def_init() {
	use = new bool*[block_number];
	def = new bool*[block_number];
	in = new bool*[block_number];
	out = new bool*[block_number];
}

void C0_Compiler::divide_block() {
	int func_count = 0;
	int i;
	mid_code *temp_mid_code;
	for (i = 0; i < this->mid_code_table_top; i++) {
		temp_mid_code = mid_code_table[i];
		switch (temp_mid_code->op) {
		case LABELOP:
			if (!this->block_begin_flag[i]) {
				Block* temp_block = new Block();
				temp_block->prenum = 0;
				temp_block->next_follow = -1;
				temp_block->next_jump = -1;
				block_table[block_number] = temp_block;
				block_begin[block_number] = i;
				block_number++;
				block_begin_flag[i] = true;
			}
			break;

		case EXITOP:
		case CALLOP:
		case GOTOOP:
		case EQLEQLOP:
		case NEQOP:
		case LSSOP:
		case LSSEQLOP:
		case GTROP:
		case GTREQLOP:
		case RETOP:
			if ((i + 1 < mid_code_table_top) && !block_begin_flag[i + 1]) {
				Block* temp_block = new Block();
				temp_block->prenum = 0;
				temp_block->next_follow = -1;
				temp_block->next_jump = -1;
				block_table[block_number] = temp_block;
				block_begin[block_number] = i + 1;
				block_number++;
				block_begin_flag[i + 1] = true;
			}
			break;

		case FUNCBEGINOP://??????BUG
			func_block_begin[func_count++] = block_begin_flag[i] ? block_number - 1 : block_number;
			if (!this->block_begin_flag[i]) {
				Block *temp_block = new Block();
				temp_block->prenum = 0;
				temp_block->next_follow = -1;
				temp_block->next_jump = -1;
				block_table[block_number] = temp_block;
				block_begin[block_number] = i;
				block_number++;
				block_begin_flag[i] = true;
			}
			if (!this->block_begin_flag[i + 1]) {
				Block *temp_block = new Block();
				temp_block->prenum = 0;
				temp_block->next_follow = -1;
				temp_block->next_jump = -1;
				block_table[block_number] = temp_block;
				block_begin[block_number] = i + 1;
				block_number++;
				block_begin_flag[i + 1] = true;
			}
			break;

		default:
			;
		}
		out_set = new bool*[block_number];
	}
}

int C0_Compiler::find_label(string* label) {
	int i;
	int index;
	for (i = 0; i < block_number; i++) {
		index = block_begin[i];
		mid_code* temp_mid_code = mid_code_table[index];
		if (temp_mid_code->op == LABELOP) {
			if (*temp_mid_code->des == *label) {
				return i;
			}
		}
	}
}

void C0_Compiler::init_block_connect() {
	int i;
	int begin;
	int end;

	for (i = 0; i < block_number; i++) {
		begin = block_begin[i];
		//如果是最后一块，则等于最后一条四元式；否则等于下一块开始
		end = i + 1 >= block_number ? mid_code_table_top : block_begin[i + 1];

		Block* preblock = block_table[i];
		int preindex = i;
		mid_code * temp_mid_code = mid_code_table[end - 1];

		switch (temp_mid_code->op) {
		case EQLEQLOP:
		case NEQOP:
		case LSSOP:
		case GTROP:
		case LSSEQLOP:
		case GTREQLOP: {
			//直接后继
			preblock->next_follow = preindex + 1;
			Block* next_block = block_table[preindex + 1];
			next_block->prearray[next_block->prenum++] = preindex;

			//跳转后继
			string* temp_label = temp_mid_code->des;
			int jump_index = -1;
			jump_index = find_label(temp_label);
			preblock->next_jump = jump_index;
			next_block = block_table[jump_index];
			next_block->prearray[next_block->prenum++] = preindex;
			break;
		}

		case EXITOP:
		case RETOP: //?函数调用分不分
			break;

		case GOTOOP: {
			string* temp_label = temp_mid_code->des;
			int next_index = -1;
			next_index = find_label(temp_label);

			//只有跳转后继
			preblock->next_jump = next_index;
			Block* next_block = block_table[next_index];
			next_block->prearray[next_block->prenum++] = preindex;
			break;
		}

		default: {
			preblock->next_follow = preindex + 1;
			Block* next_block = block_table[preindex + 1];
			next_block->prearray[next_block->prenum++] = preindex;
			break;
		}
		}
	}
}

void C0_Compiler::init_optimize() {
	block_number = 0;
	for (int i = 0; i < MAX_MID_CODE; i++) {
		block_begin_flag[i] = false;
	}
}

int C0_Compiler::get_index(int func_ref, string* name) {
	int max_length = func_sym_number[func_ref];
	symbol** sym_array = func_sym_table[func_ref];
	for (int i = 0; i < max_length; i++) {
		symbol* temp_sym = sym_array[i];
		if (*temp_sym->name == *name) {
			return i;
		}
	}
}

void C0_Compiler::data_flow() {
	int func_ref;
	int block_start;
	int block_end;
	int sym_number;

	for (int i = 0; i < func_number; i++)
	{
		func_ref = i;
		block_start = func_block_begin[i];
		block_end = i + 1 >= func_number ? block_number : func_block_begin[i + 1];
		sym_number = func_sym_number[func_ref];

		for (int j = block_end - 1; j >= block_start; j--)
		{
			use[j] = new bool[sym_number];
			def[j] = new bool[sym_number];
			in[j] = new bool[sym_number];
			out[j] = new bool[sym_number];
		}

		for (int j = block_end - 1; j >= block_start; j--)
		{

			int block_number = j;
			int begin = block_begin[j];
			int end = j + 1 >= block_number ? mid_code_table_top : block_begin[j + 1];

			for (int k = 0; k < sym_number; k++)
			{
				in[j][k] = false;
				use[j][k] = false;
				def[j][k] = false;
				out[j][k] = false;
			}

			for (int k = begin; k < end; k++)
			{
				mid_code *code = mid_code_table[k];
				switch (code->op)
				{
				case PARAVALOP:
				{
					string *des = code->des;
					if (var_is_ident(des) || var_is_temp(des))
					{

						int index = -1;
						index = get_index(func_ref, des);
						if (index != -1 && def[block_number][index] == false)
							use[block_number][index] = true;
					}
					break;
				}

				case ARRAYTOOP:
				{
					string *var_2 = code->var_2;
					string *des = code->des;
					if (var_is_ident(var_2) || var_is_temp(var_2))
					{
						int index = -1;
						index = get_index(func_ref, var_2);
						if (index != -1 && def[block_number][index] == false)
							use[block_number][index] = true;
					}
					if (var_is_temp(des) || var_is_ident(des))
					{
						int index = -1;
						index = get_index(func_ref, des);

						if (index != -1 && use[block_number][index] == false)
							def[block_number][index] = true;
					}
					break;
				}
				case ADDOP:
				case SUBOP:
				case MULTOP:
				case DIVOP:
				{
					string *var_1 = code->var_1;
					string *var_2 = code->var_2;
					string *des = code->des;
					if (var_is_ident(var_1) || var_is_temp(var_1))
					{
						int index = -1;
						index = get_index(func_ref, var_1);

						if (index != -1 && def[block_number][index] == false)
							use[block_number][index] = true;
					}
					if (var_is_ident(var_2) || var_is_temp(var_2))
					{
						int index = -1;
						index = get_index(func_ref, var_2);

						if (index != -1 && def[block_number][index] == false)
							use[block_number][index] = true;

					}

					if (var_is_ident(des) || var_is_temp(des))
					{
						int index = -1;
						index = get_index(func_ref, des);

						if (index != -1 && use[block_number][index] == false)
							def[block_number][index] = true;
					}
					break;
				}

				case EQLEQLOP:
				case NEQOP:
				case GTROP:
				case GTREQLOP:
				case LSSOP:
				case LSSEQLOP:
				{
					string *var_1 = code->var_1;
					string *var_2 = code->var_2;
					if (var_is_ident(var_1) || var_is_temp(var_1))
					{
						int index = -1;
						index = get_index(func_ref, var_1);

						if (index != -1 && def[block_number][index] == false)
							use[block_number][index] = true;
					}
					if (var_is_ident(var_2) || var_is_temp(var_2))
					{
						int index = -1;
						index = get_index(func_ref, var_2);

						if (index != -1 && def[block_number][index] == false)
							use[block_number][index] = true;
					}
					break;
				}

				case SCANFOP:
				{
					string *des = code->des;
					int index = -1;
					index = get_index(func_ref, des);

					if (index != -1 && use[block_number][index] == false)
						def[block_number][index] = true;

					break;
				}

				case PRINTFOP:
				{
					string *des = code->des;


					if (var_is_ident(des) || var_is_temp(des))
					{
						int index = -1;
						index = get_index(func_ref, des);
						if (index != -1 && !def[block_number][index])
						{
							use[block_number][index] = true;
						}
					}
					break;
				}

				case TOARRAYOP:
				{
					string *var_1 = code->var_1;
					string *var_2 = code->var_2;
					if (var_is_ident(var_1) || var_is_temp(var_1))
					{
						int index = -1;
						index = get_index(func_ref, var_1);
						if (index != -1 && def[block_number][index] == false)
							use[block_number][index] = true;
					}
					if (var_is_ident(var_2) || var_is_temp(var_2))
					{
						int index = -1;
						index = get_index(func_ref, var_2);
						if (index != -1 && def[block_number][index] == false)
							use[block_number][index] = true;
					}
					break;
				}
				}
			}

		}

		bool flag = true;
		while (flag)
		{

			flag = false;
			for (int j = block_end - 1; j >= block_start; j--)
			{
				int block_number = j;
				int sym_number = func_sym_number[func_ref];
				Block *block = block_table[block_number];

				int next_follow = block->next_follow;
				int next_jump = block->next_jump;
				if (next_follow != -1)
				{
					if (next_jump != -1)
					{

						for (int k = 0; k < sym_number; k++)
						{
							out[block_number][k] = in[next_follow][k] || in[next_jump][k];
						}
					}
					else {

						for (int k = 0; k < sym_number; k++)
						{
							out[block_number][k] = in[next_follow][k];
						}
					}
				}
				else {
					if (next_jump != -1)
					{
						for (int k = 0; k < sym_number; k++)
						{
							out[block_number][k] = in[next_jump][k];
						}
					}
				}

				for (int k = 0; k < sym_number; k++)
				{
					bool result = use[block_number][k] || (out[block_number][k] && (!def[block_number][k]));
					if (result != in[block_number][k])
					{

						flag = true;
						in[block_number][k] = result;
					}
				}

			}
		}


		int node_number = 0;
		int node_to_sym[MAX_NODE_NUMBER];
		for (int j = 0; j < sym_number; j++)
		{
			symbol *sym = func_sym_table[func_ref][j];
			if (sym->i_type == VARIDENT || sym->i_type == PARAIDENT)
			{
				node_to_sym[node_number++] = j;
			}
		}

		Conflict_Node **node_table = new Conflict_Node*[node_number];
		for (int j = 0; j < node_number; j++)
		{
			Conflict_Node *node = new Conflict_Node();

			node_table[j] = node;
			node->edge_number = 0;
			for (int k = 0; k < node_number; k++)
			{
				node->conflict[k] = false;
			}
		}

		for (int j = 0; j < node_number; j++)
		{
			Conflict_Node *node_1 = node_table[j];
			int sym_index_1 = node_to_sym[j];
			for (int k = j + 1; k < node_number; k++)
			{
				Conflict_Node *node_2 = node_table[k];
				int sym_index_2 = node_to_sym[k];
				int flag = false;

				for (int temp = block_end - 1; temp >= block_start; temp--)
				{
					bool flag_1 = in[temp][sym_index_1];
					bool flag_2 = in[temp][sym_index_2];
					flag_1 = flag_1 || def[temp][sym_index_1];
					flag_2 = flag_2 || def[temp][sym_index_2];
					if (flag_1 && flag_2) {
						flag = true;
						break;
					}
				}

				if (flag)
				{
					node_1->conflict[k] = true;
					node_1->edge_number++;
					node_2->conflict[j] = true;
					node_2->edge_number++;
				}
			}
		}

		int *order = new int[node_number];
		int orderindex = 0;

		bool *not_alloc = new bool[node_number];
		bool *out_flag = new bool[node_number];

		for (int j = 0; j < node_number; j++)
		{
			not_alloc[j] = false;
			out_flag[j] = false;
		}

		while (orderindex < node_number)
		{

			bool c_flag = false;

			for (int j = 0; j < node_number; j++)
			{
				Conflict_Node *node = node_table[j];
				if (!out_flag[j] && node->edge_number < MAX_REG_AVAILABLE)
				{
					c_flag = true;
					order[orderindex++] = j;

					out_flag[j] = true;

					for (int k = 0; k < node_number; k++)
					{
						Conflict_Node *temp = node_table[k];
						if (temp->conflict[j])
						{
							temp->edge_number--;
						}
					}
					break;
				}
			}
			if (!c_flag)
			{
				int medge_index = -1;
				int medge_number = -1;
				for (int j = 0; j < node_number; j++)
				{
					if (!out_flag[j])
					{
						Conflict_Node *node = node_table[j];
						if (node->edge_number >= medge_number)
						{
							medge_index = j;
							medge_number = node->edge_number;
						}
					}
				}

				out_flag[medge_index] = true;
				not_alloc[medge_index] = true;
				order[orderindex++] = medge_index;

				for (int k = 0; k < node_number; k++)
				{
					Conflict_Node *temp = node_table[k];
					if (temp->conflict[medge_index])
					{
						temp->edge_number--;
					}
				}
			}
		}



		bool *is_added = new bool[node_number];

		int *reg_alloc = new int[node_number];
		for (int j = 0; j < node_number; j++)
		{
			is_added[j] = false;
		}
		for (int j = node_number - 1; j >= 0; j--)
		{
			int node_index = order[j];
			Conflict_Node *node = node_table[node_index];
			is_added[node_index] = true;

			if (not_alloc[node_index] == false) {

				bool able_flag[MAX_REG_AVAILABLE];
				for (int k = 0; k < MAX_REG_AVAILABLE; k++)
				{
					able_flag[k] = true;
				}
				for (int k = 0; k < node_number; k++)
				{

					if (node->conflict[k] && is_added[k] && !not_alloc[k])
					{

						int limit_reg = reg_alloc[k];

						able_flag[limit_reg] = false;
					}
				}

				for (int k = 0; k < MAX_REG_AVAILABLE; k++)
				{
					if (able_flag[k])
					{
						reg_alloc[node_index] = k;

						int symindex = node_to_sym[node_index];
						symbol *sym = func_sym_table[func_ref][symindex];
						sym->reg_index = k;
						break;
					}
				}
			}
		}
	}
	cout << "定义使用分析完毕，寄存器分配结束" << endl;
}
