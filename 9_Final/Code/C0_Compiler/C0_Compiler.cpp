#include "C0_Compiler.h"

C0_Compiler::C0_Compiler() {
	token_number = 0;
	current_symbol_string = "";
	new_line = "";
	current_line = 0;
	current_char = '\0';
	current_pos = 0;
	begin_pos = 0;

	infile.open("15071054_test.txt", ios::in);
	outfile.open("result.txt", ios::out);
	current_symtab_file.open("current_symtable.txt", ios::out);
	errorfile.open("errorlog.txt", ios::out);
	midcodefile.open("mid_code.txt", ios::out);
	objectcodefile.open("object_code.txt", ios::out);

	block_file.open("block_info.txt", ios::out);
	data_stream_file.open("data_stream_info.txt", ios::out);
	
}
