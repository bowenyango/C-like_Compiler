#ifndef SYMBOL_TABLE_H_INCLUDED
#define SYMBOL_TABLE_H_INCLUDED

#define MAX_PARA_NUMBER 50
#define MAX_SYMBOL_NUMBER 5000
#define MAX_HASH_NUMBER 10000
#define MAX_FUNC_NUMBER 1000
#define localed true
#define not_localed false

enum ident_type {
	VARIDENT,
	CONSTIDENT,
	FUNCIDENT,
	ARRAYIDENT,
	PARAIDENT
};

enum statement_type {
	BECOME_STATE,
	IF_STATE,
	SWITCH_STATE,
	RETURN_STATE,
	SCANF_STATE,
	PRINTF_STATE
};

enum ret_type {
	INTRET,
	CHARRET,
	VOIDRET
};

typedef struct {
	std::string *name;
	int address;
	int number; //函数-参数数量 常量-常量值 数组-长度
	int which_func;
	ident_type i_type;
	ret_type r_type;
	ret_type para_list[MAX_PARA_NUMBER];
	int link;
	int func_ref;

	int reg_index;

}symbol;

static symbol *sym_table[MAX_SYMBOL_NUMBER];
static int hash_table[MAX_HASH_NUMBER];
static int func_begin[MAX_FUNC_NUMBER];
static symbol **func_sym_table[MAX_FUNC_NUMBER];
static int func_space[MAX_FUNC_NUMBER];
static int func_sym_number[MAX_FUNC_NUMBER];

static char ident_type_enum_to_string[][TOKEN_LENGTH] = {
	"VARIDENT",
	"CONSTIDENT",
	"FUNCIDENT",
	"ARRAYIDENT",
	"PARAIDENT"
};

static char ret_type_enum_to_string[][TOKEN_LENGTH] = {
	"INTRET",
	"CHARRET",
	"VOIDRET"
};


#endif // SYMBOL_TABLE_H_INCLUDED
