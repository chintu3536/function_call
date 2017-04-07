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

void Program::compile()
{
	// cout<<"Compilation Error !!!!\n";
	machine_desc_object.initialize_instruction_table();
	machine_desc_object.initialize_register_table();
	for(auto it = procedure_map.begin(); it != procedure_map.end(); it++)
	{
		(it->second)->compile();
	}
	print_assembly();
}

void Program::print_assembly()
{
	string assembly_file_name = command_options.get_file_name()+".s";
	cout<<assembly_file_name<<endl;
	std::filebuf fb;
	fb.open (assembly_file_name,std::ios::app);
	std::ostream assembly_file_buffer(&fb);
	list<Symbol_Table_Entry *> global_list = global_symbol_table.get_table();

	assembly_file_buffer<<"\t.data\n";
	for (auto it = global_list.begin(); it != global_list.end(); it++)
	{
		string name = (*it)->get_variable_name();
		Data_Type dt = (*it)->get_data_type();

		if(dt == int_data_type)
		{
			assembly_file_buffer<<name<<":\t.word 0\n";
		}
		else
		{
			assembly_file_buffer<<name<<":\t.space 8\n";
		}
	}

	for(auto it = assembly_strings.begin(); it != assembly_strings.end(); it++)
	{
		assembly_file_buffer<<it->first<<":\t.asciiz\t"<<it->second<<endl;
	}
	assembly_file_buffer<<endl;

	for(auto it = procedure_map.begin(); it != procedure_map.end(); it++)
	{
		(it->second)->print_assembly(assembly_file_buffer);
	}
}