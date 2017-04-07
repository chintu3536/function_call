
#ifndef PROGRAM_HH
#define PROGRAM_HH

#include <string>
#include <list>
#include <utility>
#include <map>

#define GLOB_SPACE "   "

using namespace std;

class Program;

extern Program program_object;

class Program
{
	Symbol_Table global_symbol_table;
	map<string, Procedure *> procedure_map;
	int string_number;
	map<string, string> assembly_strings;

public:
	Program();
	~Program();
	void delete_all();

	void add_procedure(Procedure * proc, int line);
	void set_global_table(Symbol_Table & new_global_table);

	Symbol_Table_Entry & get_symbol_table_entry(string variable);
	Data_Type get_return_type(string function_name);
	Procedure * get_procedure(string name);

	void print_sym();
	void print();

	bool variable_proc_name_check(string symbol);
	bool variable_in_proc_map_check(string symbol);
	bool variable_in_symbol_list_check(string variable);
	void global_list_in_proc_check();

	string get_new_string();
	void add_assembly_string(string asmb_str, string content);

	// compile
	void compile();
	void print_assembly();

	void optimise();
	void print_opt_icode();
};

#endif
