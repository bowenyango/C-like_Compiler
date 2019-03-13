#include <iostream>
#include <fstream>
#include <string>
#include "C0_Compiler.h"

using namespace std;

int main()
{
	C0_Compiler *compiler = new C0_Compiler();
	//compiler->words_analysis();

	string file_path;
	//cin >> file_path;
	//compiler->infile.close();
	//compiler->infile.open(file_path, ios::in);

	compiler->grammar_analysis();
	return 0;
}

