#pragma once
#define MAX_REG_AVAILABLE 14
#define MAX_MIPS_NUMBER 5000
using namespace std;

typedef struct {
	int temp_reg[10];
	int temp_reg_index;
	string* sym_reg[8];
	int sym_reg_index;
}reg_stack;



static string *SW_STR = new string("sw");
static string *LW_STR = new string("lw");
static string *LI_STR = new string("li");
static string *ADD_STR = new string("add");
static string *ADDI_STR = new string("addi");
static string *SUBI_STR = new string("subi");
static string *SUB_STR = new string("sub");
static string *BEQ_STR = new string("beq");
static string *BNE_STR = new string("bne");
static string *BGEZ_STR = new string("bgez");
static string *BGTZ_STR = new string("bgtz");
static string *BLEZ_STR = new string("blez");
static string *BLTZ_STR = new string("bltz");
static string *J_STR = new string("j");
static string *JAL_STR = new string("jal");
static string *JR_STR = new string("jr");
static string *MULT_STR = new string("mul");
static string *DIV_STR = new string("div");
static string *MFLO_STR = new string("mflo");
static string *SYSCALL_STR = new string("syscall");
static string *LA_STR = new string("la");
static string *NOP_STR = new string("nop");
static string *SLL_STR = new string("sll");
static string *MOVE_STR = new string("move");
static string *ADDU_STR = new string("addu");
static string *SP_STR = new string("$sp");
static string *FP_STR = new string("$fp");
static string *RA_STR = new string("$ra");
static string *GP_STR = new string("$gp");
static string *T9_STR = new string("$t9");
static string *T8_STR = new string("$t8");
static string *T7_STR = new string("$t7");
static string *V0_STR = new string("$v0");
static string *A0_STR = new string("$a0");
static string *R0_STR = new string("$zero");