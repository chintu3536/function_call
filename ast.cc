#include<iostream>
#include<fstream>
#include<typeinfo>

using namespace std;

#include"common-classes.hh"
#include"error-display.hh"
#include"user-options.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"procedure.hh"
#include"program.hh"

Ast::Ast()
{}

Ast::~Ast()
{}

int Ast::labelCounter = 0;
bool Ast::check_ast()
{
	stringstream msg;
	msg << "No check_ast() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Data_Type Ast::get_data_type()
{
	stringstream msg;
	msg << "No get_data_type() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Symbol_Table_Entry & Ast::get_symbol_entry()
{
	stringstream msg;
	msg << "No get_symbol_entry() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

bool Ast::is_value_zero()
{
	stringstream msg;
	msg << "No is_value_zero() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::set_data_type(Data_Type dt)
{
	stringstream msg;
	msg << "No set_data_type() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

////////////////////////////////////////////////////////////////

Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line)
{
	//ADD CODE HERE
	lhs = temp_lhs;
	rhs = temp_rhs;
	lineno = line;
	check_ast();
}

Assignment_Ast::~Assignment_Ast()
{
	//ADD CODE HERE
	delete lhs;
	delete rhs;
}

bool Assignment_Ast::check_ast()
{
	CHECK_INVARIANT((rhs != NULL), "Rhs of Assignment_Ast cannot be null");
	CHECK_INVARIANT((lhs != NULL), "Lhs of Assignment_Ast cannot be null");

	// use typeid(), get_data_type()
	//ADD CODE HERE
	if(lhs->get_data_type() == rhs->get_data_type())
	{
		return true;
	}
	CHECK_INPUT(CONTROL_SHOULD_NOT_REACH, 
		"Assignment statement data type not compatible", lineno);
}

void Assignment_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer<<"\nAsgn:"<<endl;
	file_buffer<<"\t"<<"LHS (";
	lhs->print(file_buffer);
	file_buffer<<")\n";
	file_buffer<<"\t"<<"RHS (";
	rhs->print(file_buffer);
	file_buffer<<")";
}

/////////////////////////////////////////////////////////////////

Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line)
{
	//ADD CODE HERE
	variable_symbol_entry = &(var_entry);
	CHECK_INVARIANT((variable_symbol_entry->get_variable_name() == name),
		"Variable's symbol entry is not matching its name");

}

Name_Ast::~Name_Ast()
{}

Data_Type Name_Ast::get_data_type()
{
	// refer to functions for Symbol_Table_Entry 
	//ADD CODE HERE
	return variable_symbol_entry->get_data_type();
}

Symbol_Table_Entry & Name_Ast::get_symbol_entry()
{
	//ADD CODE HERE
	return *variable_symbol_entry;
}

void Name_Ast::set_data_type(Data_Type dt)
{
	//ADD CODE HERE
	variable_symbol_entry->set_data_type(dt);
}

void Name_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer<<"Name : "<<(get_symbol_entry().get_variable_name());
}

///////////////////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::Number_Ast(DATA_TYPE number, Data_Type constant_data_type, int line)
{
	// use Ast_arity from ast.hh
	//ADD CODE HERE
	constant = number;
	set_data_type(constant_data_type);
	ast_num_child = zero_arity;
	lineno = line;
}

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::~Number_Ast()
{}

template <class DATA_TYPE>
Data_Type Number_Ast<DATA_TYPE>::get_data_type()
{
	//ADD CODE HERE
	return node_data_type;
}

template <class DATA_TYPE>
void Number_Ast<DATA_TYPE>::set_data_type(Data_Type dt)
{
	//ADD CODE HERE
	node_data_type = dt;
}

template <class DATA_TYPE>
bool Number_Ast<DATA_TYPE>::is_value_zero()
{
	//ADD CODE HERE
	if(constant == 0){
		return true;
	}
}

template <class DATA_TYPE>
void Number_Ast<DATA_TYPE>::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer<<"Num : "<<constant;
}

///////////////////////////////////////////////////////////////////////////////

Arithmetic_Expr_Ast::~Arithmetic_Expr_Ast()
{
	delete lhs;
	delete rhs;
}	

Data_Type Arithmetic_Expr_Ast::get_data_type()
{
	//ADD CODE HERE
	return node_data_type;
}

void Arithmetic_Expr_Ast::set_data_type(Data_Type dt)
{
	//ADD CODE HERE
	node_data_type = dt;
}

bool Arithmetic_Expr_Ast::check_ast()
{
	CHECK_INVARIANT((rhs != NULL), "Rhs of Arithmetic_Expr cannot be null");
	CHECK_INVARIANT((lhs != NULL), "Lhs of Arithmetic_Expr cannot be null");
	// use get_data_type(), typeid()
	//ADD CODE HERE
	if(lhs->get_data_type()==rhs->get_data_type()){
		return true;
	}
	CHECK_INPUT(CONTROL_SHOULD_NOT_REACH, "Arithmetic statement data type not compatible", lineno);
}

/////////////////////////////////////////////////////////////////////

Plus_Ast::Plus_Ast(Ast * l, Ast * r, int line)
{
	// set arity and data type
	//ADD CODE HERE
	lhs=l;
	rhs=r;
	lineno = line;
	check_ast();
	ast_num_child = binary_arity;
	set_data_type(lhs->get_data_type());
}

void Plus_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer<<"\n";
	file_buffer<<"Arith: PLUS\n";
	file_buffer<<"\t"<<"LHS (";
	lhs->print(file_buffer);
	file_buffer<<")\n";
	file_buffer<<"\t"<<"RHS (";
	rhs->print(file_buffer);
	file_buffer<<")";
}

/////////////////////////////////////////////////////////////////

Minus_Ast::Minus_Ast(Ast * l, Ast * r, int line)
{
	//ADD CODE HERE
	lhs=l;
	rhs=r;
	lineno = line;
	check_ast();
	ast_num_child = binary_arity;
	set_data_type(lhs->get_data_type());
}

void Minus_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer<<"\n";
	file_buffer<<"Arith: MINUS\n";
	file_buffer<<"\t"<<"LHS (";
	lhs->print(file_buffer);
	file_buffer<<")\n";
	file_buffer<<"\t"<<"RHS (";
	rhs->print(file_buffer);
	file_buffer<<")";
}

//////////////////////////////////////////////////////////////////

Mult_Ast::Mult_Ast(Ast * l, Ast * r, int line)
{
	//ADD CODE HERE
	lhs=l;
	rhs=r;
	lineno = line;
	check_ast();
	ast_num_child = binary_arity;
	set_data_type(lhs->get_data_type());
}

void Mult_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer<<"\n";
	file_buffer<<"Arith: MULT\n";
	file_buffer<<"\t"<<"LHS (";
	lhs->print(file_buffer);
	file_buffer<<")\n";
	file_buffer<<"\t"<<"RHS (";
	rhs->print(file_buffer);
	file_buffer<<")";
}

////////////////////////////////////////////////////////////////////

Divide_Ast::Divide_Ast(Ast * l, Ast * r, int line)
{
	//ADD CODE HERE
	lhs=l;
	rhs=r;
	lineno = line;
	check_ast();
	ast_num_child = binary_arity;
	set_data_type(lhs->get_data_type());
}

void Divide_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer<<"\n";
	file_buffer<<"Arith: DIV\n";
	file_buffer<<"\t"<<"LHS (";
	lhs->print(file_buffer);
	file_buffer<<")\n";
	file_buffer<<"\t"<<"RHS (";
	rhs->print(file_buffer);
	file_buffer<<")";
}

//////////////////////////////////////////////////////////////////////

UMinus_Ast::UMinus_Ast(Ast * l, Ast * r, int line)
{
	//ADD CODE HERE
	lhs=l;
	rhs=r;
	ast_num_child = unary_arity;
	set_data_type(lhs->get_data_type());
	lineno = line;
}

void UMinus_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer<<"\n";
	file_buffer<<"Arith: UMINUS\n";
	file_buffer<<"\t"<<"LHS (";
	lhs->print(file_buffer);
	file_buffer<<")";
}

Relational_Expr_Ast::Relational_Expr_Ast(Ast * lhs, Relational_Op rop, Ast * rhs, int line)
{
	lhs_condition = lhs;
	rhs_condition = rhs;
	rel_op = rop;
	lineno = line;
	ast_num_child = binary_arity;
	set_data_type(int_data_type);
	check_ast();
	/// ################     ARITY     #################
}

Data_Type Relational_Expr_Ast::get_data_type()
{
	return node_data_type;
}

void Relational_Expr_Ast::set_data_type(Data_Type dt)
{
	node_data_type = dt;
}

bool Relational_Expr_Ast::check_ast()
{
	CHECK_INVARIANT((rhs_condition != NULL), "Rhs of Relational_Expr cannot be null");
	CHECK_INVARIANT((lhs_condition != NULL), "Lhs of Relational_Expr cannot be null");
	// use get_data_type(), typeid()
	//ADD CODE HERE
	if(lhs_condition->get_data_type()==rhs_condition->get_data_type()){
		return true;
	}
	CHECK_INPUT(CONTROL_SHOULD_NOT_REACH, "Relational Expression data types not compatible", lineno);
}

void Relational_Expr_Ast::print(ostream & file_buffer)
{
	file_buffer<<"\nCondition: ";
	if(rel_op==less_than)
	{
		file_buffer<<"LT\n";
	}
	if(rel_op==less_equalto)
	{
		file_buffer<<"LE\n";
	}
	if(rel_op==greater_than)
	{
		file_buffer<<"GT\n";
	}
	if(rel_op==greater_equalto)
	{
		file_buffer<<"GE\n";
	}
	if(rel_op==equalto)
	{
		file_buffer<<"EQ\n";
	}
	if(rel_op==not_equalto)
	{
		file_buffer<<"NE\n";
	}
	file_buffer<<"\t"<<"LHS (";
	lhs_condition->print(file_buffer);
	file_buffer<<")\n";
	file_buffer<<"\t"<<"RHS (";
	rhs_condition->print(file_buffer);
	file_buffer<<")";
}

Boolean_Expr_Ast::Boolean_Expr_Ast(Ast * lhs, Boolean_Op bop, Ast * rhs, int line)
{
	lhs_op = lhs;
	rhs_op = rhs;
	bool_op = bop;
	lineno = line;
	if(bop == boolean_not)
		ast_num_child = unary_arity;
	else
		ast_num_child = binary_arity;
	set_data_type(rhs->get_data_type());
	check_ast();
}

Data_Type Boolean_Expr_Ast::get_data_type()
{
	return node_data_type;
}

void Boolean_Expr_Ast::set_data_type(Data_Type dt)
{
	node_data_type = dt;
}

bool Boolean_Expr_Ast::check_ast()
{
	CHECK_INVARIANT((rhs_op != NULL), "Rhs of Boolean_Expr cannot be null");
	CHECK_INVARIANT((lhs_op != NULL || bool_op == boolean_not), "Lhs of Boolean_Expr cannot be null");
	// use get_data_type(), typeid()
	//ADD CODE HERE
	if(bool_op == boolean_not || (lhs_op->get_data_type()==rhs_op->get_data_type())){
		return true;
	}
	CHECK_INPUT(CONTROL_SHOULD_NOT_REACH, "Boolean Expression data types not compatible", lineno);
}

void Boolean_Expr_Ast::print(ostream & file_buffer)
{
	file_buffer<<"\nCondition: ";
    if(bool_op==boolean_and)
    {
        file_buffer<<" AND\n";
        file_buffer<<"LHS (";
        lhs_op->print(file_buffer);
        file_buffer<<")\n";
        file_buffer<<"RHS (";
        rhs_op->print(file_buffer);
        file_buffer<<")";
    }
    if(bool_op==boolean_or)
    {
        file_buffer<<" OR\n";
        file_buffer<<"LHS (";
        lhs_op->print(file_buffer);
        file_buffer<<")\n";
        file_buffer<<"RHS (";
        rhs_op->print(file_buffer);
        file_buffer<<")";
    }
    if(bool_op==boolean_not)
    {
        file_buffer<<" NOT\n";
        file_buffer<<"RHS (";
        rhs_op->print(file_buffer);
        file_buffer<<")";
    }
}

Selection_Statement_Ast::Selection_Statement_Ast(Ast * cond, Ast* then_part, Ast* else_part, int line)
{
	this -> cond = cond;
	this -> then_part = then_part;
	this -> else_part = else_part;
	lineno = line;
	ast_num_child = ternary_arity;
	set_data_type(void_data_type);
	check_ast();
}

Data_Type Selection_Statement_Ast::get_data_type()
{
	return node_data_type;
}

void Selection_Statement_Ast::set_data_type(Data_Type dt)
{
	node_data_type = dt;
}

bool Selection_Statement_Ast::check_ast()
{
	CHECK_INVARIANT((cond != NULL), "Condition of if statement cannot be null");
	CHECK_INVARIANT((then_part != NULL), "Then then Part of if statement cannot be null");	
	CHECK_INVARIANT((else_part != NULL), "Then else Part of if statement cannot be null");

	return true;
}

void Selection_Statement_Ast::print(ostream & file_buffer)
{
	file_buffer<<"\nIF :\n";
	file_buffer<<"CONDITION (";
	cond->print(file_buffer);
	file_buffer<<")\n";
	file_buffer<<"THEN (";
	then_part->print(file_buffer);
	file_buffer<<")\n";
	file_buffer<<"ELSE (";
	else_part->print(file_buffer);
	file_buffer<<")";
}

Iteration_Statement_Ast::Iteration_Statement_Ast(Ast * cond, Ast* body, int line, bool do_form)
{
	this -> cond = cond;
	this -> body = body;
	lineno = line;
	is_do_form = do_form;
	ast_num_child = binary_arity;
	set_data_type(void_data_type);
	check_ast();
}

Data_Type Iteration_Statement_Ast::get_data_type()
{
	return node_data_type;
}

void Iteration_Statement_Ast::set_data_type(Data_Type dt)
{
	node_data_type = dt;
}

bool Iteration_Statement_Ast::check_ast()
{
	CHECK_INVARIANT((cond != NULL), "Condition of Loop can not be null");
	CHECK_INVARIANT((body != NULL), "Body of Loop can not be null");
	return true;
}

void Iteration_Statement_Ast::print(ostream & file_buffer)
{
	if(is_do_form)
	{
		file_buffer<<"\nDO (";
		body->print(file_buffer);
		file_buffer<<")\n";
		file_buffer<<"WHILE CONDITION (";
		cond->print(file_buffer);
		file_buffer<<")";
	}
	else{
		file_buffer<<"\nWHILE : \n";
		file_buffer<<"CONDITION (";
		cond->print(file_buffer);
		file_buffer<<")\n";
		file_buffer<<"BODY (";
		body->print(file_buffer);
		file_buffer<<")";
	}
}

Conditional_Operator_Ast::Conditional_Operator_Ast(Ast* cond, Ast* l, Ast* r, int line)
{
	this -> cond = cond;
	lhs = l;
	rhs = r;
	lineno = line;
	ast_num_child = ternary_arity;
	set_data_type(lhs->get_data_type());
	check_ast();
}

void Conditional_Operator_Ast::print(ostream & file_buffer)
{
	file_buffer<<"\nArith: Conditional\n";
	file_buffer<<"COND (";
	cond->print(file_buffer);
	file_buffer<<")\nLHS (";
	lhs->print(file_buffer);
	file_buffer<<")\nRHS (";
	rhs->print(file_buffer);
	file_buffer<<")";
}

Return_Ast::Return_Ast(Ast* ret_ast, int line)
{
	ret = ret_ast;
	if(ret_ast==NULL){
		ast_num_child = unary_arity;
		Data_Type dt= void_data_type;
		set_data_type(dt);
		lineno=line;
	}
	else{
		ast_num_child = unary_arity;
		set_data_type(ret_ast->get_data_type());	//TODO:
		lineno=line;
	}
}

Return_Ast::~Return_Ast()
{
	
}

void Return_Ast::set_data_type(Data_Type dt)
{
	node_data_type=dt;
}

Data_Type Return_Ast::get_data_type()
{
	return node_data_type;
}

Sequence_Ast::Sequence_Ast(int line)
{
	lineno = line;
}

Sequence_Ast::~Sequence_Ast()
{
}

void Sequence_Ast::ast_push_back(Ast *ast)
{
	statement_list.push_back(ast);
}

void Sequence_Ast::print(ostream & file_buffer)
{
	file_buffer<<"\nSequence Ast:\n";
	for(list<Ast *> ::iterator it = statement_list.begin();it!=statement_list.end();++it)
	{
		(*it)->print(file_buffer);
	}
}

template class Number_Ast<double>;
template class Number_Ast<int>;
