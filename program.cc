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

Program::Program()
{

}

Program::~Program()
{

}

void Program::delete_all()
{

}

void Program::add_procedure(Procedure * proc, int line)
{
	pair< string, Procedure *> proc_pair = make_pair(proc->get_proc_name(), proc);
	procedure_map.insert(proc_pair);
}

void Program::set_global_table(Symbol_Table & new_global_table)
{
	global_symbol_table = new_global_table;
}

Symbol_Table_Entry & Program::get_symbol_table_entry(string variable)
{
	return global_symbol_table.get_symbol_table_entry(variable);
}

Data_Type Program::get_return_type(string function_name)
{
	return (procedure_map[function_name])->get_return_type();
}

void Program::print_sym()
{
	//TODO:
}

void Program::print()
{
	//TODO:
}

bool Program::variable_proc_name_check(string symbol)
{
	return procedure_map.find(symbol)!=procedure_map.end();

}

bool Program::variable_in_symbol_list_check(string variable)
{
	return global_symbol_table.variable_in_symbol_list_check(variable);
}

void global_list_in_proc_check()
{

}