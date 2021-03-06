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

void Procedure::set_argument_list(ARG_L arg_list)
{
	argument_list = arg_list;
}

bool Procedure::same_arguments(ARG_L def_arg_list)
{
	if(def_arg_list.size()!=argument_list.size())
	{
		return false;
	}
	ARG_L::iterator it1, it2;
	for(it1=argument_list.begin(), it2=def_arg_list.begin(); it1!=argument_list.end();it1++, it2++)
	{
		if(*(it1) != *(it2))
		{
			return false;
		}
	}
	return true;
}

bool Procedure::argument_type_check(list<Data_Type> type_list)
{
	if(argument_list.size()!=type_list.size())
	{
		return false;
	}
	ARG_L::iterator it1;
	list<Data_Type>:: iterator it2;

	for(it1=argument_list.begin(), it2=type_list.begin(); it1!=argument_list.end(); it1++, it2++)
	{
		if(it1->first != *it2)
		{
			return false;
		}
	}
	return true;
}

void Procedure::add_symbol_entry(Symbol_Table_Entry &ste)
{
	local_symbol_table.push_symbol(&ste);
}

bool Procedure::variable_in_symbol_list_check(string variable)
{
	return local_symbol_table.variable_in_symbol_list_check(variable);
}

void Procedure::print_symbol_table()
{
	local_symbol_table.print(cout);
}

void Procedure::assign_offsets()
{
	local_symbol_table.assign_offsets();
}

Symbol_Table Procedure::get_symbol_table()
{
	return local_symbol_table;
}

int Procedure::local_var_size()
{
	return local_symbol_table.get_size();
}