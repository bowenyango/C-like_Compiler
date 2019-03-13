#pragma once
using namespace std;

typedef struct {
	int temp_reg[10];
	int temp_reg_index;
	string* sym_reg[8];
	int sym_reg_index;
}reg_stack;

static string* all_reg[30];

std::string *SW = new std::string("sw");
std::string *LW = new std::string("lw");
std::string *LI = new std::string("li");
std::string *ADD = new std::string("add");
std::string *ADDI = new std::string("addi");
std::string *SUBI = new std::string("subi");
std::string *SUBST = new std::string("sub");
std::string *BEQ = new std::string("beq");
std::string *BNE = new std::string("bne");
std::string *BGEZ = new std::string("bgez");
std::string *BGTZ = new std::string("bgtz");
std::string *BLEZ = new std::string("blez");
std::string *BLTZ = new std::string("bltz");
std::string *J = new std::string("j");
std::string *JAL = new std::string("jal");
std::string *JR = new std::string("jr");
std::string *MULOBJ = new std::string("mul");
std::string *DIVOBJ = new std::string("div");
std::string *MFLO = new std::string("mflo");
std::string *SYSCALL = new std::string("syscall");
std::string *LA = new std::string("la");
std::string *NOP = new std::string("nop");
std::string *SLL = new std::string("sll");
std::string *MOVE = new std::string("move");
std::string *ADDU = new std::string("addu");
std::string *SP = new std::string("$sp");
std::string *FP = new std::string("$fp");
std::string *RA = new std::string("$ra");
std::string *GP = new std::string("$gp");
std::string *T9 = new std::string("$t9");
std::string *T8 = new std::string("$t8");
std::string *T7 = new std::string("$t7");
std::string *V0 = new std::string("$v0");
std::string *A0 = new std::string("$a0");
std::string *R0 = new std::string("$zero");

