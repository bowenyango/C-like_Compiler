#ifndef SYMBOL_TABLE_H_INCLUDED
#define SYMBOL_TABLE_H_INCLUDED

#define MAX_PARA_NUMBER 10
#define MAX_SYMBOL_NUMBER 5000
#define MAX_HASH_NUMBER 10000
#define MAX_FUNC_NUMBER 1000

enum ident_type {
	VARIDENT,
	CONSTIDENT,
	FUNCIDENT,
	ARRAYIDENT,
	PARAIDENT
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
}symbol;

static symbol *sym_table[MAX_SYMBOL_NUMBER];
static int hash_table[MAX_HASH_NUMBER];
static int func_begin[MAX_FUNC_NUMBER];



#endif // SYMBOL_TABLE_H_INCLUDED
