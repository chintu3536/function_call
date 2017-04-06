#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>

using namespace std;

#include "common-classes.hh"
#include "error-display.hh"
#include "icode.hh"
#include "reg-alloc.hh"
#include "symbol-table.hh"
#include "ast.hh"
#include "program.hh"


/****************************** Class Ics_Opd *****************************/

Register_Descriptor * Ics_Opd::get_reg()
{
	//TODO

}

var Ics_Opd::get_name()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Ics_opd get_name() not defined")
}

bool Ics_Opd::is_global()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Ics_opd get_global() not defined")
}

/****************************** Class Mem_Addr_Opd *****************************/

Mem_Addr_Opd::Mem_Addr_Opd(Symbol_Table_Entry & se) 
{
	//TODO: Pass by reference doubt
	symbol_entry=&(se);
}

Mem_Addr_Opd & Mem_Addr_Opd::operator=(const Mem_Addr_Opd & rhs)
{
	//TODO
	symbol_entry=rhs.symbol_entry;
	return *this;

}

var Mem_Addr_Opd::get_name(){
	var v;
	v.name = symbol_entry->get_variable_name();
	v.is_reg=false;
	return v;
}

void Mem_Addr_Opd::print_ics_opd(ostream & file_buffer) 
{
	//TODO
	file_buffer <<symbol_entry->get_variable_name();
}

void Mem_Addr_Opd::print_asm_opd(ostream & file_buffer) 
{
	Table_Scope symbol_scope = symbol_entry->get_symbol_scope();

	CHECK_INVARIANT(((symbol_scope == local) || (symbol_scope == global)), 
			"Wrong scope value");

	if (symbol_scope == local)
	{
		int offset = symbol_entry->get_start_offset();
		file_buffer << offset << "($fp)";
	}
	else
		file_buffer << symbol_entry->get_variable_name();
}

bool Mem_Addr_Opd::is_global()
{
	Table_Scope symbol_scope = symbol_entry->get_symbol_scope();
	if(symbol_scope == global)
		return true;
	else
		return false;
}

/****************************** Class Register_Addr_Opd *****************************/

Register_Addr_Opd::Register_Addr_Opd(Register_Descriptor * reg) 
{
	//TODO
	register_description=reg;
}

Register_Descriptor * Register_Addr_Opd::get_reg()    
{ 
	//TODO 
	return register_description;
}

Register_Addr_Opd& Register_Addr_Opd::operator=(const Register_Addr_Opd& rhs)
{
	//TODO
	register_description = rhs.register_description;
	return *this;
}

var Register_Addr_Opd::get_name()
{
	var v;
	v.name = register_description->get_name();
	v.is_reg = true;
	return v;
}

void Register_Addr_Opd::print_ics_opd(ostream & file_buffer) 
{
	//TODO
	file_buffer<< register_description->get_name();
}

void Register_Addr_Opd::print_asm_opd(ostream & file_buffer) 
{
	//TODO
	file_buffer<< "$"<< register_description->get_name();
}

bool Register_Addr_Opd::is_global()
{
	return false;
}

/****************************** Class Const_Opd *****************************/

template <class DATA_TYPE>
Const_Opd<DATA_TYPE>::Const_Opd(DATA_TYPE n) 
{
	//TODO
	num=n;
}

template <class DATA_TYPE>
Const_Opd<DATA_TYPE> & Const_Opd<DATA_TYPE>::operator=(const Const_Opd<DATA_TYPE> & rhs)
{
	//TODO: type checking
	num=rhs.num;
	return *this;
}

template <class DATA_TYPE>
var Const_Opd<DATA_TYPE>::get_name() 
{
	var v;
	v.name="#";
	v.is_reg=false;
	return v;
}


template <class DATA_TYPE>
void Const_Opd<DATA_TYPE>::print_ics_opd(ostream & file_buffer) 
{
	//TODO
	file_buffer << num ;
}

template <class DATA_TYPE>
void Const_Opd<DATA_TYPE>::print_asm_opd(ostream & file_buffer) 
{
	//TODO
	file_buffer << num;
}

template <class DATA_TYPE>
bool Const_Opd<DATA_TYPE>::is_global() 
{
	return false;
}


/****************************** Class Icode_Stmt *****************************/

Instruction_Descriptor & Icode_Stmt::get_op()
{ 
	return op_desc; 
}

Ics_Opd * Icode_Stmt::get_opd1()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_Opd1 not implemented");
}

Ics_Opd * Icode_Stmt::get_opd2()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_Opd2 not implemented");
}

Ics_Opd * Icode_Stmt::get_result()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_Result not implemented");
}

void Icode_Stmt::set_opd1(Ics_Opd * ics_opd)
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method set_Opd1 not implemented");
}

void Icode_Stmt::set_opd2(Ics_Opd * ics_opd)
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method set_Opd2 not implemented");
}

void Icode_Stmt::set_result(Ics_Opd * ics_opd)
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual methos set_Result not implemented");
}

void Icode_Stmt::set_label(string s)
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method set_label not implemented");	
}

string Icode_Stmt::get_label()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_label not implemented");	
}

var Icode_Stmt::get_names_left()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_names_left not implemented");	
}

vector<var> Icode_Stmt::get_names_right()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_names_right not implemented");	
}

set<var> Icode_Stmt::get_global_list()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_global_list not implemented");	
}

/*************************** Class Move_IC_Stmt *****************************/

Move_IC_Stmt::Move_IC_Stmt(Tgt_Op op, Ics_Opd * o1, Ics_Opd * res, Stmt_Type st)
{
	CHECK_INVARIANT((machine_desc_object.spim_instruction_table[op] != NULL),
			"Instruction description in spim table cannot be null");

	op_desc = *(machine_desc_object.spim_instruction_table[op]);
	opd1 = o1;   
	result = res; 
	type=st;
}

Ics_Opd * Move_IC_Stmt::get_opd1()          { return opd1; }
Ics_Opd * Move_IC_Stmt::get_result()        { return result; }
//TODO: get_inst_op_of_ics

void Move_IC_Stmt::set_opd1(Ics_Opd * io)   { opd1 = io; }
void Move_IC_Stmt::set_result(Ics_Opd * io) { result = io; }

Move_IC_Stmt& Move_IC_Stmt::operator=(const Move_IC_Stmt& rhs)
{
	op_desc = rhs.op_desc;
	opd1 = rhs.opd1;
	result = rhs.result; 

	return *this;
}

var Move_IC_Stmt::get_names_left(){
	return result->get_name();
}

vector<var> Move_IC_Stmt::get_names_right(){
	vector<var> names;
	names.push_back(opd1->get_name());
	return names;
}

set<var> Move_IC_Stmt::get_global_list()
{
	set<var> s;
	vector<var> r = get_names_right();
	var l = get_names_left();
	if(opd1->is_global())
		s.insert(r[0]);
	if(result->is_global())
		s.insert(l);
	return s;
}

void Move_IC_Stmt::print_icode(ostream & file_buffer)
{
	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a move IC Stmt");
	CHECK_INVARIANT (result, "Result cannot be NULL for a move IC Stmt");

	string operation_name = op_desc.get_name();

	Icode_Format ic_format = op_desc.get_ic_format();

	// cout<<operation_name<<" "<<ic_format<<endl;

	switch (ic_format)
	{
	case i_r_op_o1: 
			file_buffer << "\t" << operation_name << ":    \t";
			result->print_ics_opd(file_buffer);
			file_buffer << " <- ";
			opd1->print_ics_opd(file_buffer);
			file_buffer << "\n";

			break; 

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
				"Intermediate code format not supported");
		break;
	}
}

void Move_IC_Stmt::print_assembly(ostream & file_buffer)
{
	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a move IC Stmt");
	CHECK_INVARIANT (result, "Result cannot be NULL for a move IC Stmt");
	string op_name = op_desc.get_mnemonic();

	Assembly_Format assem_format = op_desc.get_assembly_format();
	switch (assem_format)
	{
	case a_op_r_o1: 
			file_buffer << "\t" << op_name << " ";
			result->print_asm_opd(file_buffer);
			file_buffer << ", ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << "\n";

			break; 

	case a_op_o1_r: 
			file_buffer << "\t" << op_name << " ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << ", ";
			result->print_asm_opd(file_buffer);
			file_buffer << "\n";

			break; 

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Intermediate code format not supported");
		break;
	}
}


Compute_IC_Stmt::Compute_IC_Stmt(Tgt_Op op, Ics_Opd * o1, Ics_Opd * o2, Ics_Opd *res, Stmt_Type st){
	CHECK_INVARIANT((machine_desc_object.spim_instruction_table[op] != NULL),
			"Instruction description in spim table cannot be null");
	op_desc = *(machine_desc_object.spim_instruction_table[op]);
	opd1 = o1;   
	opd2 = o2;
	result = res;
	type=st;
}

Ics_Opd * Compute_IC_Stmt::get_opd1(){return opd1;}
Ics_Opd * Compute_IC_Stmt::get_opd2(){return opd2;}
Ics_Opd * Compute_IC_Stmt::get_result(){return result;}
Instruction_Descriptor & Compute_IC_Stmt::get_inst_op_of_ics(){return op_desc;}

void Compute_IC_Stmt::set_opd1(Ics_Opd * io){opd1=io;}
void Compute_IC_Stmt::set_opd2(Ics_Opd * io){opd2=io;}
void Compute_IC_Stmt::set_result(Ics_Opd * io){result=io;}

Compute_IC_Stmt & Compute_IC_Stmt::operator=(const Compute_IC_Stmt &rhs){
	op_desc = rhs.op_desc;
	opd1 = rhs.opd1;
	opd2 = rhs.opd2;
	result = rhs.result;
	return *this;
}

var Compute_IC_Stmt::get_names_left(){
	return result->get_name();
}

vector<var> Compute_IC_Stmt::get_names_right(){
	vector<var> names;
	names.push_back(opd1->get_name());
	names.push_back(opd2->get_name());
	return names;
}

set<var> Compute_IC_Stmt::get_global_list()
{
	set<var>s;
	vector<var> r = get_names_right();
	var l = get_names_left();
	if(opd1->is_global())
		s.insert(r[0]);
	if(opd2->is_global())
		s.insert(r[1]);
	if(result->is_global())
		s.insert(l);

	return s;
}

void Compute_IC_Stmt::print_icode(ostream & file_buffer){
	//TODO:
	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a compute IC Stmt");
	CHECK_INVARIANT (opd2, "Opd2 cannot be NULL for a compute IC Stmt");
	CHECK_INVARIANT (result, "result cannot be NULL for a compute IC Stmt");

	string operation_name = op_desc.get_name();
	
	Icode_Format ic_format = op_desc.get_ic_format();

	switch(ic_format)
	{
		//TODO: should there be any difference in implementation of these two case statements
		case i_r_r_op_o1:
			file_buffer << "\t" << operation_name << ":    \t";
			result->print_ics_opd(file_buffer);
			file_buffer << " <- ";
			opd1->print_ics_opd(file_buffer);
			file_buffer << " , ";
			opd2->print_ics_opd(file_buffer);
			file_buffer << "\n";
			break;

		case i_r_o1_op_o2:
			file_buffer << "\t" << operation_name << ":    \t";
			result->print_ics_opd(file_buffer);
			file_buffer << " <- ";
			opd1->print_ics_opd(file_buffer);
			file_buffer << " , ";
			opd2->print_ics_opd(file_buffer);
			file_buffer << "\n";
			break;

		default:
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
						"Intermediate code format not supported");
			break;

	}	
}

void Compute_IC_Stmt::print_assembly(ostream & file_buffer){
	//TODO:
	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a compute IC Stmt");
	CHECK_INVARIANT (opd2, "Opd2 cannot be NULL for a compute IC Stmt");
	CHECK_INVARIANT (result, "Result cannot be NULL for a compute IC Stmt");

	string op_name = op_desc.get_mnemonic();

	Assembly_Format assem_format = op_desc.get_assembly_format();
	switch (assem_format)
	{
	case a_op_r_r_o1: 
			file_buffer << "\t" << op_name << " ";
			result->print_asm_opd(file_buffer);
			file_buffer << ", ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << ", ";
			opd2->print_asm_opd(file_buffer);
			file_buffer << "\n";
			break; 

	case a_op_r_o1_o2: 
			file_buffer << "\t" << op_name << " ";
			result->print_asm_opd(file_buffer);
			file_buffer << ", ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << ", ";
			opd2->print_asm_opd(file_buffer);
			file_buffer << "\n";
			break;  

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Intermediate code format not supported");
		break;
	}
}

Control_Flow_IC_Stmt::Control_Flow_IC_Stmt(Tgt_Op op, Ics_Opd *o1, string l, Stmt_Type st){
	CHECK_INVARIANT((machine_desc_object.spim_instruction_table[op] != NULL),
			"Instruction description in spim table cannot be null");
	op_desc = *(machine_desc_object.spim_instruction_table[op]);
	opd1=o1;
	label=l;
	type=st;
}

Ics_Opd * Control_Flow_IC_Stmt::get_opd1(){return opd1;}
string Control_Flow_IC_Stmt::get_label(){return label;}

void Control_Flow_IC_Stmt::set_opd1(Ics_Opd *io){opd1=io;}
void Control_Flow_IC_Stmt::set_label(string l){label=l;}

Control_Flow_IC_Stmt & Control_Flow_IC_Stmt::operator=(const Control_Flow_IC_Stmt &rhs){
	op_desc = rhs.op_desc;
	opd1 = rhs.opd1;
	label = rhs.label;
	return *this;
}

void Control_Flow_IC_Stmt::print_icode(ostream &file_buffer){
	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a compute IC Stmt");
	CHECK_INVARIANT (!label.empty(), "result cannot be NULL for a compute IC Stmt");

	string operation_name = op_desc.get_name();
	
	Icode_Format ic_format = op_desc.get_ic_format();

	switch(ic_format){
		case i_op_o1_o2_st:
			file_buffer << "\t" << operation_name << ":    \t";
			opd1->print_ics_opd(file_buffer);
			file_buffer<<" , zero : goto "<<label<<"\n";
			break;
		default:
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
						"Intermediate code format not supported");
			break;
	}
}

void Control_Flow_IC_Stmt::print_assembly(ostream &file_buffer){	
	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a Control_Flow IC Stmt");
	CHECK_INVARIANT (!label.empty(), "Result cannot be NULL for a move IC Stmt");
	string op_name = op_desc.get_mnemonic();

	Assembly_Format assem_format = op_desc.get_assembly_format();

	switch(assem_format){
		case a_op_o1_o2_st:
			file_buffer << "\t" << op_name << " ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << ", ";
			file_buffer << "$zero, "<<label;
			file_buffer << "\n";
			break; 

		default:
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Assembly code format not supported");
			break;
	}
}

vector<var> Control_Flow_IC_Stmt::get_names_right(){
	vector<var> names;
	names.push_back(opd1->get_name());
	return names;
}

set<var>Control_Flow_IC_Stmt::get_global_list(){
	set<var> s;
	vector<var> r = get_names_right();
	if(opd1->is_global())
		s.insert(r[0]);

	return s;
}

Label_IC_Stmt::Label_IC_Stmt(Tgt_Op op, string l, Stmt_Type st){
	CHECK_INVARIANT((machine_desc_object.spim_instruction_table[op] != NULL),
			"Instruction description in spim table cannot be null");
	op_desc = *(machine_desc_object.spim_instruction_table[op]);
	label=l;
	type=st;
}

string Label_IC_Stmt::get_label(){return label;}

void Label_IC_Stmt::set_label(string l){label=l;}

void Label_IC_Stmt::print_icode(ostream & file_buffer){
	CHECK_INVARIANT(!label.empty(), "label cannot be null for a Label statement");
	string operation_name = op_desc.get_name();
	if(operation_name.compare("jump")==0){
		file_buffer<<"\tgoto "<<label<<"\n";	
	}
	else{
		file_buffer<<"\n"<<label<<":    	\n";
	}
}

void Label_IC_Stmt::print_assembly(ostream & file_buffer){
	CHECK_INVARIANT (!label.empty(), "Result cannot be NULL for a move IC Stmt");
	string op_name = op_desc.get_mnemonic();

	Assembly_Format assem_format = op_desc.get_assembly_format();

	if(op_name.compare("j")==0){
		file_buffer<<"\tj "<<label<<endl;
	}
	else{
		file_buffer<<"\n"<<label <<":    	\n";
	}

	// switch(assem_format){
	// 	case a_op_o1:
	// 		file_buffer << "\t" << op_name << " ";
	// 		file_buffer << " "<<label;
	// 		file_buffer << "\n";
	// 		break; 

	// 	case a_op_st:
	// 		file_buffer << label <<":\n";
	// 		break;

	// 	default:
	// 		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Assembly code format not supported");
	// 		break;
	// }
}

set<var> Label_IC_Stmt::get_global_list()
{
	set<var> s;
	return s;
}

/******************************* Class Code_For_Ast ****************************/

Code_For_Ast::Code_For_Ast()
{
	//TODO
}

Code_For_Ast::Code_For_Ast(list<Icode_Stmt *> & ic_l, Register_Descriptor * reg)
{
	//TODO
	ics_list = ic_l;
	result_register = reg;
}

void Code_For_Ast::append_ics(Icode_Stmt & ic_stmt)
{
	//TODO
	ics_list.push_back(&ic_stmt);
}

list<Icode_Stmt *> & Code_For_Ast::get_icode_list()  
{ 
	//TODO 
	return ics_list;
}

Register_Descriptor * Code_For_Ast::get_reg()
{
	//TODO
	return result_register;
}

void Code_For_Ast::set_reg(Register_Descriptor * reg)
{
	//TODO
	result_register=reg;
}

Code_For_Ast& Code_For_Ast::operator=(const Code_For_Ast& rhs)
{
	//TODO
	ics_list=rhs.ics_list;
	result_register=rhs.result_register;
	return *this;
}

/************************ class Instruction_Descriptor ********************************/

Tgt_Op Instruction_Descriptor::get_op()                   	{ return inst_op; }
string Instruction_Descriptor::get_name()                       { return name; }
string Instruction_Descriptor::get_mnemonic()                   { return mnemonic; }
string Instruction_Descriptor::get_ic_symbol()                  { return ic_symbol; }
Icode_Format Instruction_Descriptor::get_ic_format()            { return ic_format; }
Assembly_Format Instruction_Descriptor::get_assembly_format()   { return assem_format; }

Instruction_Descriptor::Instruction_Descriptor (Tgt_Op op, string temp_name, string temp_mnemonic, 
						string ic_sym, Icode_Format ic_form, Assembly_Format as_form)
{
	inst_op = op;
	name = temp_name; 
	mnemonic = temp_mnemonic;
	ic_symbol = ic_sym;
	ic_format = ic_form;
	assem_format = as_form;
}

Instruction_Descriptor::Instruction_Descriptor()
{
	inst_op = nop;
	name = "";
	mnemonic = "";
	ic_symbol = "";
	ic_format = i_nsy;
	assem_format = a_nsy;
}

IC_Block::IC_Block(string l, int num){
	label=l;
	block_no=num;
	next = NULL;
	jump = NULL;
	sequ = NULL;
	is_goto = false;
   	is_branch = false;
}

//TODO: destructor
IC_Block::~IC_Block(){
	delete next;
}

void IC_Block::append_ics(Icode_Stmt * ics){
	ics_list.push_back(ics);
}

void IC_Block::set_label(string l){
	label=l;
}

void IC_Block::set_block_no(int num){
	block_no=num;
}

void IC_Block::set_next(IC_Block *next_block_ptr){
	next = next_block_ptr;
}

IC_Block * IC_Block::get_next(){
	return next;
}

void IC_Block::set_sequ(IC_Block *sequ_block_ptr){
	sequ = sequ_block_ptr;
}

IC_Block * IC_Block::get_sequ(){
	return sequ;
}

void IC_Block::set_jump(IC_Block *jump_block_ptr){
	jump = jump_block_ptr;
}

void IC_Block::set_is_goto(bool b){
	is_goto = b;
}

bool IC_Block::get_is_goto(){
	return is_goto;
}

void IC_Block::set_is_branch(bool b){
	is_branch = b;
}

bool IC_Block::get_is_branch(){
	return is_branch;
}

void IC_Block::set_goto_label(string l){
	goto_label = l;
}

string IC_Block::get_goto_label(){
	return goto_label;
}

void IC_Block::populate_gen_kill(){
	Gen.clear(); kill.clear(); right.clear();
	for(list<Icode_Stmt *> ::iterator it=ics_list.begin(); it!=ics_list.end(); it++){
		Stmt_Type st =(*it)->get_stmt_type();
		if(st==label_stmt || st==direct_jump){
			continue;
		}
		else{
			vector<var> names = (*it)->get_names_right();
			for(int i=0;i<names.size();i++){
				if(kill.find(names[i])!=kill.end()){
					if(names[i].name != "#")
						right.insert(names[i]);
				}
				else{
					if(names[i].name != "#")
						Gen.insert(names[i]);
				}
			}
			if(st != branch){
				var left_name = (*it)->get_names_left();
				kill.insert(left_name);
			}
		}
	}
} 

void IC_Block::print_gen()
{
	cout<<"\nGen : ";
	for(set<var> ::iterator it=Gen.begin(); it!=Gen.end(); it++){
		cout<<(*it).name<<" ";
	}
	cout<<endl;
}

void IC_Block::print_kill()
{
	cout<<"\nKill : ";
	for(set<var> ::iterator it=kill.begin(); it!=kill.end(); it++){
		cout<<(*it).name<<" ";
	}
	cout<<endl;
}

void IC_Block::print_in()
{
	cout<<"\nIn : ";
	for(set<var> ::iterator it=in.begin(); it!=in.end(); it++){
		cout<<(*it).name<<" ";
	}
	cout<<endl;
}

void IC_Block::print_out()
{
	cout<<"\nOut : ";
	for(set<var> ::iterator it=out.begin(); it!=out.end(); it++){
		cout<<(*it).name<<" ";
	}
	cout<<endl;
}

void IC_Block::clear_in()
{
	in.clear();
}

void IC_Block::clear_out()
{
	out.clear();
}

set<var> IC_Block::get_in()
{
	return in;
}

set<var> IC_Block::get_gen(){
	return Gen;
}

set<var> IC_Block::get_right(){
	return right;
}

set<var> IC_Block::get_out(){
	return out;
}
set<var> IC_Block::get_kill(){
	return kill;
}

bool IC_Block::populate_in_out(set<var> global)
{
	set<var> out_minus_kill = set_diff(out, kill);
	set<var> in_k = set_uni(Gen, out_minus_kill);
	set<var> out_k;
	if(next != NULL && jump != NULL)
	{
		out_k = set_uni(next->get_in(), jump->get_in());
	}
	else if(next != NULL)
	{
		out_k = next->get_in();
	}
	else if(jump != NULL)
	{
		out_k = jump->get_in();
	}
	else
	{

	}

	if(sequ == NULL)
	{

		out_k = set_uni(out_k, global);
	}

	if(in_k.size() != in.size() || out_k.size() != out.size())
	{
		out = out_k;
		in = in_k;
		return true;
	}
	else
	{
		return false;
	}
}

////////////REMOVE

list<Icode_Stmt *> IC_Block::get_ics_list(){
	return ics_list;
}

void IC_Block::set_ics_list(list<Icode_Stmt *> l){
	// cout<<"\nSetting ...\n";
	ics_list = l;
}

int IC_Block::get_block_no(){
	return block_no;
}

IC_Block * IC_Block::get_jump(){
	return jump;
}

template class Const_Opd<int>;
template class Const_Opd<double>;

set<var> set_diff(set<var>s1, set<var>s2)
{
	set<var> result;

	set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(),
    inserter(result, result.end()));
    return result;
}

set<var> set_uni(set<var>s1, set<var>s2)
{
	set<var> result;

	set_union(s1.begin(), s1.end(), s2.begin(), s2.end(),
    inserter(result, result.end()));
    return result;
}