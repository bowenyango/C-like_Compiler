#include "C0_Compiler.h"
#include <iostream>
using namespace std;


void C0_Compiler::words_analysis(){
    while(infile.eof() != true){
        next_line();

        do{
            next_letter();
            skip_spacetab();

            begin_pos = current_pos;

            if(is_plus()){
                temp = new token();
                temp->line = current_line;
                temp->pos = begin_pos;
                temp->t_type = PLUS;
                temp->t_value.letter = '+';
                temp->t_value_type = LETTER;
                addin_table();
            }
            else if(is_sub()){
                temp = new token();
                temp->line = current_line;
                temp->pos = begin_pos;
                temp->t_type = SUB;
                temp->t_value.letter = '-';
                temp->t_value_type = LETTER;
                addin_table();

            }
            else if(is_mult()){
                temp = new token();
                temp->line = current_line;
                temp->pos = begin_pos;
                temp->t_type = MULT;
                temp->t_value.letter = '*';
                temp->t_value_type = LETTER;
                addin_table();
            }
            else if(is_div()){
                temp = new token();
                temp->line = current_line;
                temp->pos = begin_pos;
                temp->t_type = DIV;
                temp->t_value.letter = '/';
                temp->t_value_type = LETTER;
                addin_table();
            }
            else if(is_become()){
                next_letter();
                if(is_become()){
                    temp = new token();
                    temp->line = current_line;
                    temp->pos = begin_pos;
                    temp->t_type = EQLEQL;
                    temp->t_value.pstring = new string("==");
                    temp->t_value_type = PSTRING;
                    addin_table();
                }
                else{
                    back_letter();
                    temp = new token();
                    temp->line = current_line;
                    temp->pos = begin_pos;
                    temp->t_type = BECOME;
                    temp->t_value.letter = '=';
                    temp->t_value_type = LETTER;
                    addin_table();
                }
            }
            else if(is_exm()){
                next_letter();
                if(is_become()){
                    temp = new token();
                    temp->line = current_line;
                    temp->pos = begin_pos;
                    temp->t_type = NEQ;
                    temp->t_value.pstring = new string("!=");
                    temp->t_value_type = PSTRING;
                    addin_table();
                }
                else{
                    ERROR(NOT_EQUAL_ERROR); //
                }
            }
            else if(is_gtr()){
                next_letter();
                if(is_become()){
                    temp = new token();
                    temp->line = current_line;
                    temp->pos = begin_pos;
                    temp->t_type = GTREQL;
                    temp->t_value.pstring = new string(">=");
                    temp->t_value_type = PSTRING;
                    addin_table();
                }
                else{
                    back_letter();
                    temp = new token();
                    temp->line = current_line;
                    temp->pos = begin_pos;
                    temp->t_type = GTR;
                    temp->t_value.letter = '>';
                    temp->t_value_type = LETTER;
                    addin_table();
                }
            }
            else if(is_lss()){
                next_letter();
                if(is_become()){
                    temp = new token();
                    temp->line = current_line;
                    temp->pos = begin_pos;
                    temp->t_type = LSSEQL;
                    temp->t_value.pstring = new string("<=");
                    temp->t_value_type = PSTRING;
                    addin_table();
                }
                else{
                    back_letter();
                    temp = new token();
                    temp->line = current_line;
                    temp->pos = begin_pos;
                    temp->t_type = LSS;
                    temp->t_value.letter = '<';
                    temp->t_value_type = LETTER;
                    addin_table();
                }
            }
            else if(is_int()){
                int temp_number = 0;
                do{
                    temp_number = 10 * temp_number + current_char - '0';
                    next_letter();
                }while(is_int());

                temp = new token();
                temp->line = current_line;
                temp->pos = begin_pos;
                temp->t_type = INT;
                temp->t_value.number = temp_number;
                temp->t_value_type = NUMBER;
                addin_table();
                back_letter();
            }
            else if(is_char()){
                int mark;
                clear_token();
                do{
                    current_token.push_back(current_char);
                    next_letter();
                }while(is_char() || is_int());

                back_letter();

                std::transform(current_token.begin(), current_token.end(), current_token.begin(), ::tolower);

                if(mark = is_symbol()){
                    temp = new token();
                    temp->line = current_line;
                    temp->pos = begin_pos;
                    temp->t_type = (token_type)mark;
                    temp->t_value.pstring = new string(current_token);
                    temp->t_value_type = PSTRING;
                    addin_table();
                }
                else{
                    temp = new token();
                    temp->line = current_line;
                    temp->pos = begin_pos;
                    temp->t_type = IDENT;
                    temp->t_value.pstring = new string(current_token);
                    temp->t_value_type = PSTRING;
                    addin_table();
                }
            }
            else if(is_double_quot()){
                clear_token();
                bool error = false;

                do{
                    next_letter();
                    if(is_instring()){
                        current_token.push_back(current_char);
                    }
                    else if(is_double_quot()){
                        break;
                    }
                    else if(is_null()){ //如果到了句尾还没有第二个双引号
                        ERROR(DOUBLE_QUOT_ERROR);
                        error = true;
                        break;
                    }
                    else{ //如果有非法字符
                        ERROR(NOT_LEGAL_STRING);
                        error = true;
                    }
                }while(!is_double_quot());

                if(!error){
                    temp = new token();
                    temp->line = current_line;
                    temp->pos = begin_pos;
                    temp->t_type = STRING;
                    temp->t_value.pstring = new string(current_token);
                    temp->t_value_type = PSTRING;
                    addin_table();
                }
            }
            else if(is_single_quot()){
                next_letter();
                char temp_char;
                if(is_plus() || is_sub() || is_mult() || is_div() || is_int() || is_char()){
                    temp_char = current_char;
                    next_letter();
                    if(!is_single_quot()){ //没有第二个单引号
                        ERROR(SINGLE_QUOT_ERROR);
                        back_letter();
                    }
                    else{
                        temp = new token();
                        temp->line = current_line;
                        temp->pos = begin_pos;
                        temp->t_type = CHAR;
                        temp->t_value.letter = temp_char;
                        temp->t_value_type = LETTER;
                        addin_table();
                    }
                }
                else if(is_single_quot()){
                    ERROR(NULL_CHAR_ERROR); //两个单引号中间为空
                }
                else{ //非法字符
                    ERROR(UNIDENTIFIED_CHAR);
                    next_letter();
                    if(!is_single_quot())
                        back_letter();
                }
            }
            else if(is_left_par()){
                temp = new token();
                temp->line = current_line;
                temp->pos = begin_pos;
                temp->t_type = LEFT_PAR;
                temp->t_value.letter = '(';
                temp->t_value_type = LETTER;
                addin_table();
            }
            else if(is_right_par()){
                temp = new token();
                temp->line = current_line;
                temp->pos = begin_pos;
                temp->t_type = RIGHT_PAR;
                temp->t_value.letter = ')';
                temp->t_value_type = LETTER;
                addin_table();
            }
            else if(is_left_bracket()){
                temp = new token();
                temp->line = current_line;
                temp->pos = begin_pos;
                temp->t_type = LEFT_BRACKET;
                temp->t_value.letter = '[';
                temp->t_value_type = LETTER;
                addin_table();
            }
            else if(is_right_bracket()){
                temp = new token();
                temp->line = current_line;
                temp->pos = begin_pos;
                temp->t_type = RIGHT_BRACKET;
                temp->t_value.letter = ']';
                temp->t_value_type = LETTER;
                addin_table();
            }
            else if(is_left_brace()){
                temp = new token();
                temp->line = current_line;
                temp->pos = begin_pos;
                temp->t_type = LEFT_BRACE;
                temp->t_value.letter = '{';
                temp->t_value_type = LETTER;
                addin_table();
            }
            else if(is_right_brace()){
                temp = new token();
                temp->line = current_line;
                temp->pos = begin_pos;
                temp->t_type = RIGHT_BRACE;
                temp->t_value.letter = '}';
                temp->t_value_type = LETTER;
                addin_table();
            }
            else if(is_comma()){
                temp = new token();
                temp->line = current_line;
                temp->pos = begin_pos;
                temp->t_type = COMMA;
                temp->t_value.letter = ',';
                temp->t_value_type = LETTER;
                addin_table();
            }
            else if(is_colon()){
                temp = new token();
                temp->line = current_line;
                temp->pos = begin_pos;
                temp->t_type = COLON;
                temp->t_value.letter = ':';
                temp->t_value_type = LETTER;
                addin_table();
            }
            else if(is_semicolon()){
                temp = new token();
                temp->line = current_line;
                temp->pos = begin_pos;
                temp->t_type = SEMICOLON;
                temp->t_value.letter = ';';
                temp->t_value_type = LETTER;
                addin_table();
            }

        }while(current_char != '\0');
    }

    print_token_table();
}



void C0_Compiler::clear_token(){
    current_token = "";
}

void C0_Compiler::next_letter(){
    current_char = new_line[current_pos];
    current_pos ++;
}

void C0_Compiler::back_letter(){
    current_pos --;
    current_char = new_line[current_pos - 1];
}

void C0_Compiler::next_line(){
    int temp;
    getline(infile, new_line);
    current_line ++;
    current_pos = 0;
}

void C0_Compiler::skip_spacetab(){
    while(is_space() || is_tab())
        next_letter();
}

bool C0_Compiler::is_plus(){
    if(current_char == '+')
        return true;
    else
        return false;
}

bool C0_Compiler::is_sub(){
    if(current_char == '-')
        return true;
    else
        return false;
}

bool C0_Compiler::is_mult(){
    if(current_char == '*')
        return true;
    else
        return false;
}

bool C0_Compiler::is_div(){
    if(current_char == '/')
        return true;
    else
        return false;
}

bool C0_Compiler::is_become(){
    if(current_char == '=')
        return true;
    else
        return false;
}

bool C0_Compiler::is_exm(){
    if(current_char == '!')
        return true;
    else
        return false;
}

bool C0_Compiler::is_gtr(){
    if(current_char == '>')
        return true;
    else
        return false;
}

bool C0_Compiler::is_lss(){
    if(current_char == '<')
        return true;
    else
        return false;
}

bool C0_Compiler::is_int(){
    if(current_char >= '0' && current_char <= '9')
        return true;
    else
        return false;
}

bool C0_Compiler::is_char(){
    if(current_char == '_' || current_char >= 'a' && current_char <= 'z'
       || current_char >= 'A' && current_char <= 'Z')
        return true;
    else
        return false;
}

bool C0_Compiler::is_instring(){
    if(current_char >= 32 && current_char <= 126 && !is_double_quot())
        return true;
    else
        return false;
}

bool C0_Compiler::is_single_quot(){
    if(current_char == '\'')
        return true;
    else
        return false;
}

bool C0_Compiler::is_double_quot(){
    if(current_char == '"')
        return true;
    else
        return false;
}

bool C0_Compiler::is_left_par(){
    if(current_char == '(')
        return true;
    else
        return false;
}

bool C0_Compiler::is_right_par(){
    if(current_char == ')')
        return true;
    else
        return false;
}

bool C0_Compiler::is_left_bracket(){
    if(current_char == '[')
        return true;
    else
        return false;
}

bool C0_Compiler::is_right_bracket(){
    if(current_char == ']')
        return true;
    else
        return false;
}

bool C0_Compiler::is_left_brace(){
    if(current_char == '{')
        return true;
    else
        return false;
}

bool C0_Compiler::is_right_brace(){
    if(current_char == '}')
        return true;
    else
        return false;
}

bool C0_Compiler::is_comma(){
    if(current_char == ',')
        return true;
    else
        return false;
}

bool C0_Compiler::is_colon(){
    if(current_char == ':')
        return true;
    else
        return false;
}

bool C0_Compiler::is_semicolon(){
    if(current_char == ';')
        return true;
    else
        return false;
}

bool C0_Compiler::is_space(){
    if(current_char == ' ')
        return true;
    else
        return false;
}

bool C0_Compiler::is_tab(){
    if(current_char == '\t')
        return true;
    else
        return false;
}

bool C0_Compiler::is_null(){
    if(current_char == '\0')
        return true;
    else
        return false;
}

int C0_Compiler::is_symbol(){
    int i;
    for(i = 0; i <= WHILE_SYMBOL - CONST_SYMBOL; i++){
        if(token_symbol[i] == current_token)
            return (CONST_SYMBOL + i);
    }

    return 0;
}

void C0_Compiler::addin_table(){
    token_table[token_number] = temp;
    token_number++;
}

void C0_Compiler::ERROR(int err_typ){
    cout << "ERROR HAPPEN IN " << current_line << "_" << begin_pos;
    errorfile << "ERROR HAPPEN IN " << current_line << "_" << begin_pos;
    switch(err_typ){
    case SINGLE_QUOT_ERROR:
        cout << " ERROR TYPE: MISS SINGLE QUOTATION." << endl;
        errorfile << " ERROR TYPE: MISS SINGLE QUOTATION." << endl;
        break;
    case DOUBLE_QUOT_ERROR:
        cout << " ERROR TYPE: MISS DOUBLE QUOTATION." << endl;
        errorfile << " ERROR TYPE: MISS DOUBLE QUOTATION." << endl;
        break;
    case UNIDENTIFIED_CHAR:
        cout << " ERROR TYPE: NOT PERMITTED CHAR." << endl;
        errorfile << " ERROR TYPE: NOT PERMITTED CHAR." << endl;
        break;
    case NOT_EQUAL_ERROR:
        cout << " ERROR TYPE: MISS = AFTER !." << endl;
        errorfile << " ERROR TYPE: MISS = AFTER !." << endl;
        break;
    case NOT_LEGAL_STRING:
        cout << " ERROR TYPE: NOT PERMITTED STRING." << endl;
        errorfile << " ERROR TYPE: NOT PERMITTED STRING." << endl;
        break;
    case NULL_CHAR_ERROR:
        cout << " ERROR TYPE: '' CHAR IS NULL" << endl;
        errorfile << " ERROR TYPE: '' CHAR IS NULL" << endl;
        break;
    default:
        cout << " UNDEFINED ERROR." << endl;
        errorfile << " UNDEFINED ERROR." << endl;
    }
}

void C0_Compiler::print_token_table(){
    int i;
    for(i = 0; i < token_number; i++){
        temp = token_table[i];

//        cout << token_type_enum_to_string[temp->t_type] << "\t";
        outfile << i + 1 << "\t" << temp->line << "\t" << temp->pos << "\t" << token_type_enum_to_string[temp->t_type] << "\t";
        if(temp->t_value_type == LETTER){
//            cout << temp->t_value.letter << endl;
            outfile << temp->t_value.letter << endl;
        }
        else if(temp->t_value_type == NUMBER){
//            cout << temp->t_value.number << endl;
            outfile << temp->t_value.number << endl;
        }
        else{
//            cout << *temp->t_value.pstring << endl;
            outfile << *temp->t_value.pstring << endl;
        }
    }

}
