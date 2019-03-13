#pragma once
#define MAX_MID_CODE 10000


using namespace std;

enum mid_op {
	CALLOP, //º¯Êýµ÷ÓÃ call des
	RETOP, //returnÓï¾ä return des
	ARRAYTOOP, // des = var_1[var_2]
	TOARRAYOP, // des[var_2] = var_1;
	ADDOP, SUBOP, MULTOP, DIVOP, // des = var_1 +-*/ var_2
	EQLEQLOP, NEQOP, GTROP, GTREQLOP, LSSOP, LSSEQLOP,// des = var_1 == var_2
	LABELOP, // des:
	GOTOOP, BNZOP, BZOP, // GOTO des..     BNZ var_1 des..      BZ var_1 des..
	FUNCBEGINOP,// var_1(int/char/void) des
	SCANFOP, PRINTFOP,// scanf var_2 des..     printf var_2 des..
	EXITOP, // exit
	NOTOP, //nop
	PARAVALOP, // push des
	ASSIGNOP // des = var_1
};

typedef struct {
	mid_op op;
	string *var_1;
	string *var_2;
	string *des;
}mid_code;

static string* ZERO_VAR = new string("0");