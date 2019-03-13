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
	//compiler->words_analysis();


	string file_path;
	cin >> file_path;
	compiler->infile.close();
	compiler->infile.open(file_path, ios::in);
	

	compiler->grammar_analysis();
	cout << "midcode output begin" << endl;
	compiler->output_mid_code_tofile();
	cout << "mips begin" << endl;
	compiler->mips_generate();
	

	return 0;
}

