#include <iostream>
#include <fstream>
#include <typeinfo>
#include <map>

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

void Procedure::compile()
{
	sequence_ast->compile();
}

void Procedure::print_icode(ostream & file_buffer)
{
	//TODO: New label
	sequence_ast->print_icode(file_buffer);
}

void Procedure::print_assembly(ostream & file_buffer)
{
	//TODO: New label
	file_buffer<<"\t.text\n";
	file_buffer<<"\t.globl "<<name<<"\n";
	file_buffer<<name<<":\n\n";
	print_prologue(file_buffer);
	sequence_ast->print_assembly(file_buffer);
	print_epilogue(file_buffer);
}

void Procedure::print_prologue(ostream & file_buffer)
{
	file_buffer<<"# Prologue begins\n";
	file_buffer<<"sw $ra, 0($sp)\t";
	file_buffer<<"#save the return address\n";
	file_buffer<<"sw $fp, -4($sp)\t";
	file_buffer<<"#save the frame pointer\n";
	file_buffer<<"sub $sp, $sp, 8\t";
	file_buffer<<"#update the frame pointer\n";
	file_buffer<<"\n";
	file_buffer<<"sub $sp, $sp "<<local_var_size()+8<<"\t";
	file_buffer<<"#Make space for the locals\n";
	file_buffer<<"#Prologue ends\n";
}

void Procedure::print_epilogue(ostream & file_buffer)
{
	file_buffer<<"# Epilogue begins\n";
	file_buffer<<"epilouge_"<<name<<":\n";
	file_buffer<<"add $sp, $sp, "<<local_var_size()+8<<"\n";
	file_buffer<<"lw $fp, -4($sp)\n";
	file_buffer<<"lw $ra, 0($sp)\n";
	file_buffer<<"jr\t$31\t";
	file_buffer<<"# Jump back to the called procedure\n";
}