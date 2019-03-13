#include <iostream>
#include <fstream>
#include "C0_Compiler.h"

using namespace std;

int main()
{
    C0_Compiler *compiler = new C0_Compiler();
    compiler->words_analysis();

    return 0;
}
