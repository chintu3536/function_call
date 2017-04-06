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

Procedure::Procedure(Data_Type proc_return_type, string proc_name, int line)
{
	return_type = proc_return_type;
	name = proc_name;
	lineno = line;
}

Procedure::~Procedure()
{

}

string Procedure::get_proc_name()
{
	return name;
}

void Procedure::set_sequence_ast(Sequence_Ast & sa)
{
	sequence_ast = &(sa);
}

void Procedure::set_local_list(Symbol_Table & new_list)
{
	local_symbol_table = new_list;
}

Data_Type Procedure::get_return_type()
{
	return return_type;
}

Symbol_Table_Entry & Procedure::get_symbol_table_entry(string variable_name)
{
	return local_symbol_table.get_symbol_table_entry(variable_name);
}

bool Procedure::variable_in_symbol_list_check(string variable)
{
	return local_symbol_table.variable_in_symbol_list_check(variable);
}
