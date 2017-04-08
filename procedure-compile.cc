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
	cout<<"I am gonna compile "<<name<<endl;
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
	file_buffer<<"#Prologue begins\n";
	file_buffer<<"\tsub $sp, $sp, 4\t# first make space on stack\n";
	file_buffer<<"\tsw $ra, 0($sp)\n";
	file_buffer<<"\tsub $sp, $sp, 4\n";
	file_buffer<<"\tsw $fp, 0($sp)\n";
	file_buffer<<"\tmove $fp, $sp\n";
	file_buffer<<"\tsub $sp, $sp, "<<local_var_size()<<"\n";
	file_buffer<<"#Prologue ends\n";
}

void Procedure::print_epilogue(ostream & file_buffer)
{
	file_buffer<<"# Epilogue begins\n";
	file_buffer<<"epilouge_"<<name<<":\n";
	file_buffer<<"\tadd $sp, $sp, "<<local_var_size()<<"\n";
	file_buffer<<"\tlw $fp, 0($sp)\n";
	file_buffer<<"\tadd $sp, $sp, 4\n";
	file_buffer<<"\tlw $ra, 0($sp)\n";
	file_buffer<<"\tadd $sp, $sp, 4\n";
	if (name != "main")
		file_buffer<<"\tjr $ra\t#Jump back to the called procedure\n";
	else
	{
		file_buffer<<"\tli $v0, 10\n\tsyscall\n";
	}
}