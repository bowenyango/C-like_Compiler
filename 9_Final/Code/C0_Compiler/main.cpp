#include <iostream>
#include <fstream>
#include <string>
#include "C0_Compiler.h"

using namespace std;

void test(string* temp) {
	*temp = string("test");
}

int main()
{

	C0_Compiler *compiler = new C0_Compiler();
	C0_Compiler *compiler_optimize = new C0_Compiler();

	string file_path;
	cin >> file_path;
	compiler->infile.close();
	compiler->infile.open(file_path, ios::in);

	//origin
	compiler->grammar_analysis();
	cout << "midcode output begin" << endl;
	compiler->output_mid_code_tofile("15071054_杨伯文_优化前中间代码.txt");
	cout << "mips begin" << endl;
	compiler->mips_generate("15071054_杨伯文_优化前目标代码.txt");

	//Optimize
	compiler_optimize->infile.close();
	compiler_optimize->infile.open(file_path, ios::in);

	compiler_optimize->grammar_analysis();
	compiler_optimize->init_optimize();
	compiler_optimize->divide_block();
	compiler_optimize->init_block_connect();
	compiler_optimize->use_def_init();
	compiler_optimize->data_flow();
	compiler_optimize->output_mid_code_tofile("15071054_杨伯文_优化后中间代码.txt");
	compiler_optimize->mips_generate("15071054_杨伯文_优化后目标代码.txt");


	return 0;
}

