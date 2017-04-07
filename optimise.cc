#include <iostream>
#include <fstream>
#include <typeinfo>

using namespace std;

#include "common-classes.hh"
#include "error-display.hh"
#include "user-options.hh"
#include "icode.hh"
#include "reg-alloc.hh"
#include "symbol-table.hh"
#include "ast.hh"
#include "procedure.hh"
#include "program.hh"


void Procedure::optimise_icode(){
	//sequence_ast->optimise_icode();
}

void Procedure::print_opt_icode(ostream & file_buffer){
	// file_buffer<<"  Procedure: "<<name<<endl;
	// sequence_ast->print_opt_icode(file_buffer);
}

// void User_Options::create_dce_buffer(){
// 	dce_file_name=source_file+".dce";
// 	dce_file_buffer.open(dce_file_name, ofstream::out);
// 	dce_buffer = &(dce_file_buffer);
// }

// ostream & User_Options::get_dce_buffer(){
// 	return *(dce_buffer);
// }

void Program::optimise(){
	// procedure->optimise_icode();
}

void Program::print_opt_icode(){
	// string source_file = command_options.get_file_name();
	// string dce_file_name=source_file+".dce";
	// ofstream dce_file_buffer;
	// dce_file_buffer.open(dce_file_name, ofstream::out);
	// ostream * dce_buffer = &(dce_file_buffer);
	// procedure->print_opt_icode(*dce_buffer);
	// dce_file_buffer.close();
}