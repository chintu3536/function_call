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

typedef list<Icode_Stmt*>::iterator ic_list_it;

Code_For_Ast & Ast::create_store_stmt(Register_Descriptor * store_register)
{
	stringstream msg;
	msg << "No create_store_stmt() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::print_assembly()
{
	stringstream msg;
	msg << "No print_assembly() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::print_icode()
{
	stringstream msg;
	msg << "No print_icode() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

////////////////////////////////////////////////////////////////

Code_For_Ast & Assignment_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null in Assignment_Ast");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null in Assignment_Ast");

	Code_For_Ast & load_stmt = rhs->compile();

	Register_Descriptor * load_register = load_stmt.get_reg();
	CHECK_INVARIANT(load_register, "Load register cannot be null in Assignment_Ast");
	load_register->set_use_for_expr_result();

	Code_For_Ast store_stmt = lhs->create_store_stmt(load_register);

	CHECK_INVARIANT((load_register != NULL), "Load register cannot be null in Assignment_Ast");
	load_register->reset_use_for_expr_result();

	// Store the statement in ic_list

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (load_stmt.get_icode_list().empty() == false)
		ic_list = load_stmt.get_icode_list();

	if (store_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), store_stmt.get_icode_list());

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, load_register);

	return *assign_stmt;
}


/////////////////////////////////////////////////////////////////

Code_For_Ast & Name_Ast::compile()
{
	CHECK_INVARIANT((variable_symbol_entry != NULL), "variable_symbol_entry cannot be null in Name_Ast");

	Register_Use_Category r;
	Tgt_Op op;
	Register_Descriptor * out_reg;
	if (get_data_type() == int_data_type){
		r = gp_data;
		out_reg = machine_desc_object.get_new_register<gp_data>();
	}
	else{
		r = float_reg;
		out_reg = machine_desc_object.get_new_register<float_reg>();
	}

	CHECK_INVARIANT((out_reg != NULL), "Result Register cannot be null in Name_Ast");

	out_reg->set_use_for_expr_result();

	Ics_Opd * name_opd = new Mem_Addr_Opd(*variable_symbol_entry);
	Ics_Opd * out_res = new Register_Addr_Opd(out_reg);

	Data_Type dt = get_data_type();

	if (dt == int_data_type){
		op = load;
	}else{
		op = load_d;
	}

	Stmt_Type st = simple;
	Icode_Stmt * name = new Move_IC_Stmt(op, name_opd, out_res, st);
	CHECK_INVARIANT((name != NULL), "Load Statement cannot be null in Name_Ast");


	out_reg->reset_use_for_expr_result();

	list<Icode_Stmt *> name_list;
	name_list.push_back(name);

	Code_For_Ast * name_code = new Code_For_Ast(name_list, out_reg);

	return *name_code;
}

Code_For_Ast & Name_Ast::create_store_stmt(Register_Descriptor * store_register)
{
	CHECK_INVARIANT((variable_symbol_entry != NULL), "variable_symbol_entry cannot be null in Name_Ast");
	CHECK_INVARIANT((store_register != NULL), "Store_Register cannot be null in Name_Ast");

	Ics_Opd * name_opd = new Mem_Addr_Opd(*variable_symbol_entry);
	Ics_Opd * out_res = new Register_Addr_Opd(store_register);

	Data_Type dt = get_data_type();

	Tgt_Op op;
	if (dt == int_data_type){
		op = store;
	}else{
		op = store_d;
	}

	Stmt_Type st = simple;
	Icode_Stmt * store_stmt = new Move_IC_Stmt(op, out_res, name_opd, st);

	CHECK_INVARIANT((store_stmt != NULL), "Store Statement cannot be null in create_store_stmt");

	// TODO:

	list<Icode_Stmt*> store;
	store.push_back(store_stmt);

	Code_For_Ast * store_code = new Code_For_Ast(store, store_register);
	return *store_code;
}


///////////////////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
Code_For_Ast & Number_Ast<DATA_TYPE>::compile()
{
	Tgt_Op op;
	Register_Descriptor * out_reg;
	Ics_Opd * num_opd;
	Data_Type dt = get_data_type();
	if (dt == int_data_type){
		out_reg = machine_desc_object.get_new_register<gp_data>();
		num_opd = new Const_Opd<int>(constant);
	}
	else{
		out_reg = machine_desc_object.get_new_register<float_reg>();
		num_opd = new Const_Opd<double>(constant);
	}

	CHECK_INVARIANT((out_reg != NULL), "Result Register cannot be null in Number_Ast");

	out_reg->set_use_for_expr_result();

	Ics_Opd * out_res = new Register_Addr_Opd(out_reg);

	if (dt == int_data_type){
		op = imm_load;
	}else{
		op = imm_load_d;
	}

	Stmt_Type st = simple;
	Icode_Stmt * num = new Move_IC_Stmt(op, num_opd, out_res, st);

	CHECK_INVARIANT((num != NULL), "Load Statement cannot be null in Number_Ast");

	out_reg->reset_use_for_expr_result();

	list<Icode_Stmt *> num_list;
	num_list.push_back(num);

	Code_For_Ast * num_code = new Code_For_Ast(num_list, out_reg);

	return *num_code;
}

///////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Relational_Expr_Ast::compile()
{
	CHECK_INVARIANT((lhs_condition != NULL), "Lhs cannot be null in Relational_Expr_Ast");
	CHECK_INVARIANT((rhs_condition != NULL), "Rhs cannot be null in Relational_Expr_Ast");

	Code_For_Ast & lhs_stmt = lhs_condition->compile();
	Register_Descriptor * lhs_reg = lhs_stmt.get_reg();
	lhs_reg->set_use_for_expr_result();

	Code_For_Ast & rhs_stmt = rhs_condition->compile();
	Register_Descriptor * rhs_reg = rhs_stmt.get_reg();
	rhs_reg->set_use_for_expr_result();

	Tgt_Op op;
	
	switch(rel_op){
		case less_equalto:
			op = sle;
			break;

		case less_than:
			op = slt;
			break;

		case greater_than:
			op = sgt;
			break;

		case greater_equalto:
			op = sge;
			break;

		case equalto:
			op = seq;
			break;

		case not_equalto:
			op = sne;
			break;
	}

	Register_Descriptor * out_reg = machine_desc_object.get_new_register<gp_data>();
	out_reg->set_use_for_expr_result();

	Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_reg);
	Ics_Opd * rhs_opd = new Register_Addr_Opd(rhs_reg);
	Ics_Opd * out_res = new Register_Addr_Opd(out_reg);
	Stmt_Type st = simple;
	Icode_Stmt * rel = new Compute_IC_Stmt(op, lhs_opd, rhs_opd, out_res, st);

	lhs_reg->reset_use_for_expr_result();
	rhs_reg->reset_use_for_expr_result();
	out_reg->reset_use_for_expr_result();

	list<Icode_Stmt *> rel_list;

	rel_list.insert(rel_list.end(), lhs_stmt.get_icode_list().begin(), lhs_stmt.get_icode_list().end());
	rel_list.insert(rel_list.end(), rhs_stmt.get_icode_list().begin(), rhs_stmt.get_icode_list().end());
	rel_list.push_back(rel);

	Code_For_Ast * rel_code = new Code_For_Ast(rel_list, out_reg);

	return *rel_code;
}

//////////////////////////////////////////////////////////////////////

Code_For_Ast & Boolean_Expr_Ast::compile()
{

	if (bool_op == boolean_not)
	{
		CHECK_INVARIANT((lhs_op == NULL), "Lhs should be null in Boolean_Not_Ast");
		CHECK_INVARIANT((rhs_op != NULL), "Rhs cannot be null in Boolean_Not_Ast");

		Register_Descriptor *one_reg = machine_desc_object.get_new_register<gp_data>();
		one_reg->set_use_for_expr_result();

		Ics_Opd *one_reg_opd = new Register_Addr_Opd(one_reg);
		Ics_Opd *const_opd = new Const_Opd<int>(1);

		Tgt_Op op_i=imm_load;
		Stmt_Type st = simple;
		Icode_Stmt *one_stmt = new Move_IC_Stmt(op_i, const_opd, one_reg_opd, st);

		Code_For_Ast & rhs_stmt = rhs_op->compile();
		Register_Descriptor * rhs_reg = rhs_stmt.get_reg();
		rhs_reg->set_use_for_expr_result();

		Tgt_Op op = not_t;

		Register_Descriptor * out_reg = machine_desc_object.get_new_register<gp_data>();
		out_reg->set_use_for_expr_result();

		Ics_Opd * rhs_opd = new Register_Addr_Opd(rhs_reg);
		Ics_Opd * out_res = new Register_Addr_Opd(out_reg);

		st = simple;
		Icode_Stmt * not_stmt = new Compute_IC_Stmt(op, rhs_opd, one_reg_opd, out_res, st);

		rhs_reg->reset_use_for_expr_result();
		out_reg->reset_use_for_expr_result();
		one_reg->reset_use_for_expr_result();


		list<Icode_Stmt *> not_list;
		not_list.push_back(one_stmt);
		not_list.insert(not_list.end(), rhs_stmt.get_icode_list().begin(), rhs_stmt.get_icode_list().end());
		not_list.push_back(not_stmt);

		Code_For_Ast * not_code = new Code_For_Ast(not_list, out_reg);

		return *not_code;
	}

	CHECK_INVARIANT((lhs_op != NULL), "Lhs cannot be null in Boolean_Expr_Ast");
	CHECK_INVARIANT((rhs_op != NULL), "Rhs cannot be null in Boolean_Expr_Ast");

	Code_For_Ast & lhs_stmt = lhs_op->compile();
	Register_Descriptor * lhs_reg = lhs_stmt.get_reg();
	lhs_reg->set_use_for_expr_result();

	Code_For_Ast & rhs_stmt = rhs_op->compile();
	Register_Descriptor * rhs_reg = rhs_stmt.get_reg();
	rhs_reg->set_use_for_expr_result();

	Tgt_Op op;
	
	switch(bool_op){
		case boolean_and:
			op = and_t;
			break;

		case boolean_or:
			op = or_t;
			break;
	}

	Register_Descriptor * out_reg = machine_desc_object.get_new_register<gp_data>();
	out_reg->set_use_for_expr_result();

	Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_reg);
	Ics_Opd * rhs_opd = new Register_Addr_Opd(rhs_reg);
	Ics_Opd * out_res = new Register_Addr_Opd(out_reg);
	Stmt_Type st = simple;
	Icode_Stmt * bool_stmt = new Compute_IC_Stmt(op, lhs_opd, rhs_opd, out_res, st);

	lhs_reg->reset_use_for_expr_result();
	rhs_reg->reset_use_for_expr_result();	~Func_Call_Ast();

	out_reg->reset_use_for_expr_result();

	list<Icode_Stmt *> bool_list;

	bool_list.insert(bool_list.end(), lhs_stmt.get_icode_list().begin(), lhs_stmt.get_icode_list().end());
	bool_list.insert(bool_list.end(), rhs_stmt.get_icode_list().begin(), rhs_stmt.get_icode_list().end());
	bool_list.push_back(bool_stmt);

	Code_For_Ast * bool_code = new Code_For_Ast(bool_list, out_reg);

	return *bool_code;
}
///////////////////////////////////////////////////////////////////////

Code_For_Ast & Selection_Statement_Ast::compile()
{
	CHECK_INVARIANT((cond != NULL), "Condition cannot be null in Selection_Statement_Ast");
	CHECK_INVARIANT((then_part != NULL), "Then part cannot be null in Selection_Statement_Ast");
	CHECK_INVARIANT((else_part != NULL), "Else part cannot be null in Selection_Statement_Ast");

	Code_For_Ast & cond_stmt = cond -> compile();
	Register_Descriptor * cond_reg = cond_stmt.get_reg();
	CHECK_INVARIANT((cond_reg != NULL), "Cond Reg cannot be null in Selection_Statement_Ast");
	cond_reg->set_use_for_expr_result();

	Code_For_Ast & then_stmt = then_part -> compile();
	Code_For_Ast & else_stmt = else_part -> compile();

	string label0 = get_new_label();
	string label1 = get_new_label();
	

	Ics_Opd * cond_opd = new Register_Addr_Opd(cond_reg);
	Tgt_Op comp = beq;
	Stmt_Type st = branch;
	Icode_Stmt * cond_ic_stmt = new Control_Flow_IC_Stmt(comp, cond_opd, label0, st);

	Tgt_Op jump = j;
	st = direct_jump;
	Icode_Stmt * j_label1 = new Label_IC_Stmt(jump, label1, st);

	Tgt_Op lab = label;
	st = label_stmt;
	Icode_Stmt * lab0 = new Label_IC_Stmt(lab, label0, st);
	Icode_Stmt * lab1 = new Label_IC_Stmt(lab, label1, st);


	list<Icode_Stmt*> cond_list = cond_stmt.get_icode_list();
	list<Icode_Stmt*> then_list = then_stmt.get_icode_list();
	list<Icode_Stmt*> else_list = else_stmt.get_icode_list();

	list<Icode_Stmt*> select_list;
	select_list.insert(select_list.end(), cond_list.begin(), cond_list.end());
	select_list.push_back(cond_ic_stmt);
	select_list.insert(select_list.end(), then_list.begin(), then_list.end());
	select_list.push_back(j_label1);
	select_list.push_back(lab0);
	select_list.insert(select_list.end(), else_list.begin(), else_list.end());
	select_list.push_back(lab1);

	Register_Descriptor * out_reg = machine_desc_object.get_new_register<gp_data>();
	out_reg->set_use_for_expr_result();

	Code_For_Ast * select_code = new Code_For_Ast(select_list, out_reg);

	cond_reg->reset_use_for_expr_result();
	out_reg->reset_use_for_expr_result();

	return * select_code;
}

///////////////////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Iteration_Statement_Ast::compile()
{
	CHECK_INVARIANT((cond != NULL), "Condition cannot be null in Iteration_Statement_Ast");
	CHECK_INVARIANT((body != NULL), "Body cannot be null in Iteration_Statement_Ast");

	string label0;
	string label1;

	
	
	Code_For_Ast & cond_stmt = cond -> compile();

	label0=get_new_label();
	label1=get_new_label();

	Register_Descriptor * cond_reg = cond_stmt.get_reg();
	CHECK_INVARIANT((cond_reg != NULL), "Cond Reg cannot be null in Selection_Statement_Ast");
	cond_reg->set_use_for_expr_result();

	Code_For_Ast & body_stmt = body -> compile();
	

	Tgt_Op comp = bne;
	Ics_Opd * cond_opd = new Register_Addr_Opd(cond_reg);
	Stmt_Type st = branch;
	Icode_Stmt * cond_ic_stmt = new Control_Flow_IC_Stmt(comp, cond_opd, label0, st);

	Tgt_Op lab = label;
	st = label_stmt;
	Icode_Stmt * lab0 = new Label_IC_Stmt(lab, label0, st);
	Icode_Stmt * lab1 = new Label_IC_Stmt(lab, label1, st);


	Tgt_Op jump = j;
	st = direct_jump;
	Icode_Stmt * j_label1 = new Label_IC_Stmt(jump, label1, st);

	list<Icode_Stmt*> cond_list = cond_stmt.get_icode_list();
	list<Icode_Stmt*> body_list = body_stmt.get_icode_list();

	list<Icode_Stmt*> it_list;
	if(!is_do_form)
	{
		it_list.push_back(j_label1);
	}

	it_list.push_back(lab0);
	it_list.insert(it_list.end(), body_list.begin(), body_list.end());
	it_list.push_back(lab1);
	it_list.insert(it_list.end(), cond_list.begin(), cond_list.end());
	it_list.push_back(cond_ic_stmt);

	Register_Descriptor * out_reg = machine_desc_object.get_new_register<gp_data>();
	out_reg->set_use_for_expr_result();

	Code_For_Ast * it_code = new Code_For_Ast(it_list, out_reg);

	cond_reg->reset_use_for_expr_result();
	out_reg->reset_use_for_expr_result();

	return * it_code;
}

///////////////////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Plus_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null in Assignment_Ast");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null in Assignment_Ast");

	Code_For_Ast & lhs_stmt = lhs->compile();
	Register_Descriptor * lhs_reg = lhs_stmt.get_reg();
	lhs_reg->set_use_for_expr_result();

	Code_For_Ast & rhs_stmt = rhs->compile();
	Register_Descriptor * rhs_reg = rhs_stmt.get_reg();
	rhs_reg->set_use_for_expr_result();

	Register_Descriptor * out_reg;
	Tgt_Op op;
	if (lhs -> get_data_type() == int_data_type){
		op = add;
		out_reg = machine_desc_object.get_new_register<gp_data>();
	}
	else{
		op = add_d;
		out_reg = machine_desc_object.get_new_register<float_reg>();
	}

	out_reg->set_use_for_expr_result();

	Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_reg);
	Ics_Opd * rhs_opd = new Register_Addr_Opd(rhs_reg);
	Ics_Opd * out_res = new Register_Addr_Opd(out_reg);
	Stmt_Type st = simple;
	Icode_Stmt * add = new Compute_IC_Stmt(op, lhs_opd, rhs_opd, out_res, st);

	lhs_reg->reset_use_for_expr_result();
	rhs_reg->reset_use_for_expr_result();
	out_reg->reset_use_for_expr_result();

	list<Icode_Stmt *> add_list;

	add_list.insert(add_list.end(), lhs_stmt.get_icode_list().begin(), lhs_stmt.get_icode_list().end());
	add_list.insert(add_list.end(), rhs_stmt.get_icode_list().begin(), rhs_stmt.get_icode_list().end());
	add_list.push_back(add);

	Code_For_Ast * add_code = new Code_For_Ast(add_list, out_reg);

	return *add_code;
}

/////////////////////////////////////////////////////////////////

Code_For_Ast & Minus_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null in Assignment_Ast");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null in Assignment_Ast");

	Code_For_Ast & lhs_stmt = lhs->compile();
	Register_Descriptor * lhs_reg = lhs_stmt.get_reg();
	lhs_reg->set_use_for_expr_result();

	Code_For_Ast & rhs_stmt = rhs->compile();
	Register_Descriptor * rhs_reg = rhs_stmt.get_reg();
	rhs_reg->set_use_for_expr_result();

	Tgt_Op op;
	Register_Descriptor * out_reg;
	if (lhs -> get_data_type() == int_data_type){
		op = sub;
		out_reg = machine_desc_object.get_new_register<gp_data>();
	}
	else{
		op = sub_d;
		out_reg = machine_desc_object.get_new_register<float_reg>();
	}
	out_reg->set_use_for_expr_result();

	Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_reg);
	Ics_Opd * rhs_opd = new Register_Addr_Opd(rhs_reg);
	Ics_Opd * out_res = new Register_Addr_Opd(out_reg);
	Stmt_Type st = simple;
	Icode_Stmt * sub = new Compute_IC_Stmt(op, lhs_opd, rhs_opd, out_res, st);


	lhs_reg->reset_use_for_expr_result();
	rhs_reg->reset_use_for_expr_result();
	out_reg->reset_use_for_expr_result();

	list<Icode_Stmt *> sub_list;

	sub_list.insert(sub_list.end(), lhs_stmt.get_icode_list().begin(), lhs_stmt.get_icode_list().end());
	sub_list.insert(sub_list.end(), rhs_stmt.get_icode_list().begin(), rhs_stmt.get_icode_list().end());
	sub_list.push_back(sub);

	Code_For_Ast * sub_code = new Code_For_Ast(sub_list, out_reg);

	return *sub_code;
}

//////////////////////////////////////////////////////////////////

Code_For_Ast & Mult_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null in Assignment_Ast");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null in Assignment_Ast");

	Code_For_Ast & lhs_stmt = lhs->compile();
	Register_Descriptor * lhs_reg = lhs_stmt.get_reg();
	lhs_reg->set_use_for_expr_result();

	Code_For_Ast & rhs_stmt = rhs->compile();
	Register_Descriptor * rhs_reg = rhs_stmt.get_reg();
	rhs_reg->set_use_for_expr_result();

	Tgt_Op op;
	Register_Descriptor * out_reg;
	if (lhs -> get_data_type() == int_data_type){
		op = mult;
		out_reg = machine_desc_object.get_new_register<gp_data>();
	}
	else{
		op = mult_d;
		out_reg = machine_desc_object.get_new_register<float_reg>();
	}
	out_reg->set_use_for_expr_result();

	Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_reg);
	Ics_Opd * rhs_opd = new Register_Addr_Opd(rhs_reg);
	Ics_Opd * out_res = new Register_Addr_Opd(out_reg);
	Stmt_Type st = simple;
	Icode_Stmt * mult = new Compute_IC_Stmt(op, lhs_opd, rhs_opd, out_res, st);

	lhs_reg->reset_use_for_expr_result();
	rhs_reg->reset_use_for_expr_result();
	out_reg->reset_use_for_expr_result();

	list<Icode_Stmt *> mult_list;

	mult_list.insert(mult_list.end(), lhs_stmt.get_icode_list().begin(), lhs_stmt.get_icode_list().end());
	mult_list.insert(mult_list.end(), rhs_stmt.get_icode_list().begin(), rhs_stmt.get_icode_list().end());
	mult_list.push_back(mult);

	Code_For_Ast * mult_code = new Code_For_Ast(mult_list, out_reg);

	return *mult_code;
}

////////////////////////////////////////////////////////////////////

Code_For_Ast & Conditional_Operator_Ast::compile()
{
	CHECK_INVARIANT((cond != NULL), "Condition can not be null in Conditional_Operator_Ast");
	CHECK_INVARIANT((lhs != NULL), "lhs can not be null in Conditional_Operator_Ast");
	CHECK_INVARIANT((rhs != NULL), "rhs can not be null in Conditional_Operator_Ast");

	Code_For_Ast & cond_stmt = cond->compile();
	Register_Descriptor * cond_reg = cond_stmt.get_reg();
	cond_reg->set_use_for_expr_result();

	Code_For_Ast & lhs_stmt = lhs->compile();
	Register_Descriptor * lhs_reg = lhs_stmt.get_reg();
	lhs_reg->set_use_for_expr_result();

	Code_For_Ast & rhs_stmt = rhs->compile();
	Register_Descriptor * rhs_reg = rhs_stmt.get_reg();
	rhs_reg->set_use_for_expr_result();

	list<Icode_Stmt*> cond_list = cond_stmt.get_icode_list();

	Ics_Opd * cond_opd = new Register_Addr_Opd(cond_reg);
	Tgt_Op comp = beq;
	Stmt_Type st = branch; //TODO:
	string label0 = get_new_label();
	Icode_Stmt * cond_ic_stmt = new Control_Flow_IC_Stmt(comp, cond_opd, label0, st);

	Register_Descriptor * out_reg;
	if (lhs->get_data_type() == int_data_type){
		out_reg = machine_desc_object.get_new_register<gp_data>();
	}else{
		out_reg = machine_desc_object.get_new_register<float_reg>();
	}
	out_reg->set_use_for_expr_result();
	Ics_Opd * out_res = new Register_Addr_Opd(out_reg);

	
	Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_reg);
	list<Icode_Stmt*> lhs_list = lhs_stmt.get_icode_list();

	Register_Descriptor * zero_reg = machine_desc_object.spim_register_table[zero];
	Ics_Opd * zero_opd = new Register_Addr_Opd(zero_reg);
	Tgt_Op or_op = or_t;
	st = simple;
	Icode_Stmt * or_stmt1 = new Compute_IC_Stmt(or_op, lhs_opd, zero_opd, out_res, st);

	Tgt_Op jump = j;
	st = direct_jump;
	string label1 = get_new_label();
	Icode_Stmt * j_label1 = new Label_IC_Stmt(jump, label1, st);
	
	Tgt_Op lab = label;
	st = label_stmt;
	Icode_Stmt * lab0 = new Label_IC_Stmt(lab, label0, st);
	
	Ics_Opd * rhs_opd = new Register_Addr_Opd(rhs_reg);
	list<Icode_Stmt*> rhs_list = rhs_stmt.get_icode_list();

	st = simple;
	Icode_Stmt * or_stmt2 = new Compute_IC_Stmt(or_op, rhs_opd, zero_opd, out_res, st);

	st = label_stmt;
	Icode_Stmt * lab1 = new Label_IC_Stmt(lab, label1, st);

	cond_reg->reset_use_for_expr_result();
	out_reg->reset_use_for_expr_result();
	lhs_reg->reset_use_for_expr_result();
	rhs_reg->reset_use_for_expr_result();

	list<Icode_Stmt *> cond_asgn_list;
	cond_asgn_list.insert(cond_asgn_list.end(), cond_list.begin(), cond_list.end());
	cond_asgn_list.push_back(cond_ic_stmt);
	cond_asgn_list.insert(cond_asgn_list.end(), lhs_list.begin(), lhs_list.end());
	cond_asgn_list.push_back(or_stmt1);
	cond_asgn_list.push_back(j_label1);
	cond_asgn_list.push_back(lab0);
	cond_asgn_list.insert(cond_asgn_list.end(), rhs_list.begin(), rhs_list.end());
	cond_asgn_list.push_back(or_stmt2);
	cond_asgn_list.push_back(lab1);

	Code_For_Ast * cond_asgn_code = new Code_For_Ast(cond_asgn_list, out_reg);
	return *cond_asgn_code;
}


////////////////////////////////////////////////////////////////////

Code_For_Ast & Divide_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null in Assignment_Ast");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null in Assignment_Ast");

	Code_For_Ast & lhs_stmt = lhs->compile();
	Register_Descriptor * lhs_reg = lhs_stmt.get_reg();
	lhs_reg->set_use_for_expr_result();

	Code_For_Ast & rhs_stmt = rhs->compile();
	Register_Descriptor * rhs_reg = rhs_stmt.get_reg();
	rhs_reg->set_use_for_expr_result();

	Tgt_Op op;
	Register_Descriptor * out_reg;
	if (lhs -> get_data_type() == int_data_type){
		op = divd;
		out_reg = machine_desc_object.get_new_register<gp_data>();
	}
	else{
		op = div_d;
		out_reg = machine_desc_object.get_new_register<float_reg>();
	}
	out_reg->set_use_for_expr_result();

	Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_reg);
	Ics_Opd * rhs_opd = new Register_Addr_Opd(rhs_reg);
	Ics_Opd * out_res = new Register_Addr_Opd(out_reg);
	Stmt_Type st = simple;
	Icode_Stmt * div = new Compute_IC_Stmt(op, lhs_opd, rhs_opd, out_res, st);

	lhs_reg->reset_use_for_expr_result();
	rhs_reg->reset_use_for_expr_result();
	out_reg->reset_use_for_expr_result();

	list<Icode_Stmt *> div_list;

	div_list.insert(div_list.end(), lhs_stmt.get_icode_list().begin(), lhs_stmt.get_icode_list().end());
	div_list.insert(div_list.end(), rhs_stmt.get_icode_list().begin(), rhs_stmt.get_icode_list().end());
	div_list.push_back(div);

	Code_For_Ast * div_code = new Code_For_Ast(div_list, out_reg);

	return *div_code;
}


//////////////////////////////////////////////////////////////////////

Code_For_Ast & UMinus_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null in Assignment_Ast");

	Code_For_Ast & lhs_stmt = lhs->compile();
	Register_Descriptor * lhs_reg = lhs_stmt.get_reg();
	lhs_reg->set_use_for_expr_result();

	Tgt_Op op;
	Data_Type dt; // TODO: 
	Register_Descriptor * out_reg;
	if (lhs -> get_data_type() == int_data_type){
		op = uminus;
		dt = int_data_type;
		out_reg = machine_desc_object.get_new_register<gp_data>();
	}
	else{
		op = uminus_d;
		dt = double_data_type;
		out_reg = machine_desc_object.get_new_register<float_reg>();
	}
	out_reg->set_use_for_expr_result();

	Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_reg);

	Ics_Opd * out_res = new Register_Addr_Opd(out_reg);
	Stmt_Type st = simple;
	Icode_Stmt * umin = new Move_IC_Stmt(op, lhs_opd, out_res, st);

	lhs_reg->reset_use_for_expr_result();
	out_reg->reset_use_for_expr_result();

	list<Icode_Stmt *> umin_list;

	umin_list.insert(umin_list.end(), lhs_stmt.get_icode_list().begin(), lhs_stmt.get_icode_list().end());
	umin_list.push_back(umin);

	Code_For_Ast * umin_code = new Code_For_Ast(umin_list, out_reg);

	return *umin_code;
}

Code_For_Ast & Return_Ast::compile()
{
	//TODO:
}

Code_For_Ast & Print_Ast::compile()
{

}

Code_For_Ast & String_Ast::compile()
{

}

Code_For_Ast & Func_Call_Ast::compile()
{
	
}

Code_For_Ast & Sequence_Ast::compile()
{	
	for(list<Ast*>::iterator it = statement_list.begin(); it != statement_list.end(); it++)
	{
		CHECK_INVARIANT(((*it) != NULL), "Ast cannot be null in Sequence_Ast");
		Code_For_Ast & seq_stmt = (*it) -> compile();
		list<Icode_Stmt *> seq_list = seq_stmt.get_icode_list(); 
		sa_icode_list.insert(sa_icode_list.end(), seq_list.begin(), seq_list.end());
	}	

	Code_For_Ast *seq_code=new Code_For_Ast(sa_icode_list, NULL);
	return *seq_code;

}

void Sequence_Ast::print_assembly(ostream & file_buffer)
{
	for(list<Icode_Stmt*>::iterator it = sa_icode_list.begin(); it != sa_icode_list.end(); it ++)
	{
		(*it)->print_assembly(file_buffer);
	}
}

void Sequence_Ast::print_icode(ostream & file_buffer)
{	
	for(ic_list_it it = sa_icode_list.begin(); it != sa_icode_list.end(); it ++)
	{
		(*it)->print_icode(file_buffer);
	}
}

void Sequence_Ast::print_opt_icode(ostream &file_buffer)
{
	file_buffer<<"  Intermediate Code Statements\n";
    list<Icode_Stmt*>::iterator it;
    for(it=opt_icode_list.begin(); it!=opt_icode_list.end(); it++)
    {
        (*it)->print_icode(file_buffer);
    }
}

void Sequence_Ast::optimise_icode(){
	set_global();

	int curr_block_no = 0;
	map<string, IC_Block *> block_of_label;

	IC_Block * start_block = new IC_Block("none", curr_block_no);
	IC_Block * curr_block = start_block;

  // Iterate through icode statements and create basic blocks with next pointers.
	for(ic_list_it it = sa_icode_list.begin(); it != sa_icode_list.end(); it ++)
	{
		Stmt_Type st = (*it)->get_stmt_type();
		if (st == simple)
		{
			curr_block->append_ics(*it);
		}
		else if (st == branch)
		{
			string l = (*it)->get_label();

			curr_block->set_is_branch(true);
			curr_block->set_goto_label(l);
			curr_block->append_ics(*it);
			IC_Block * next_block = new IC_Block("none", ++curr_block_no);
			curr_block->set_next(next_block);
			curr_block->set_sequ(next_block);
			curr_block = next_block;
		}
		else if (st == label_stmt)
		{
			string l = (*it)->get_label();

			if((curr_block->get_ics_list()).size()!=0){
				IC_Block * next_block = new IC_Block(l, ++curr_block_no);
				curr_block->set_next(next_block);
				curr_block->set_sequ(next_block);
				block_of_label[l] = next_block;
				curr_block = next_block;
				curr_block->append_ics(*it);
			}
			else{
				block_of_label[l] = curr_block;
				curr_block->append_ics(*it);
			}
		}
		else if (st == direct_jump)
		{
			string l = (*it)->get_label();

			curr_block->set_is_goto(true);
			curr_block->set_goto_label(l);
			curr_block->append_ics(*it);
			IC_Block * next_block = new IC_Block("none", ++curr_block_no);
			curr_block->set_sequ(next_block);
			curr_block = next_block;
		}
		else
		{
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Unknown IC_Stmt Type");
		}
	}

	// Set jump edges to blocks containing goto or branch statements
	for (IC_Block * icb = start_block; icb != NULL; icb = icb->get_sequ())
	{
		if (icb->get_is_goto() || icb->get_is_branch())
		{
			string goto_l = icb->get_goto_label();
			IC_Block * goto_block = block_of_label[goto_l];
			icb->set_jump(goto_block);
		}
	}

	bool dead_found=true;
	int count = 0;
	while(dead_found){
		for (IC_Block * icb = start_block; icb != NULL; icb = icb->get_sequ()){
			icb->populate_gen_kill();
		}

		for( IC_Block * icb = start_block; icb != NULL; icb = icb->get_sequ())
		{
			icb->clear_in(); icb->clear_out();
		}

		bool set_changed=true;
		while(set_changed){
			set_changed = false;
			for( IC_Block * icb = start_block; icb != NULL; icb = icb->get_sequ())
			{
				bool block_changed = icb->populate_in_out(global);
				if(block_changed)
					set_changed = true;
			}
		}

		dead_found=false;
		for (IC_Block * icb = start_block; icb != NULL; icb = icb->get_sequ()){
			set<var> dead_var;
			dead_var=set_diff(icb->get_kill(), icb->get_out());
			list<Icode_Stmt *> block_ics_list = icb->get_ics_list();
			for(list<Icode_Stmt *> ::iterator it=block_ics_list.begin();it!=(block_ics_list).end();it++){

				var v = (*it)->get_names_left();
				if (dead_var.find(v) != dead_var.end())
				{
					list<Icode_Stmt*>::iterator it1 = ++it; it--;
					bool found_eq;
					if(it1==block_ics_list.end()){
						found_eq=false;
					}
					for(; it1 != block_ics_list.end(); it1++){
						found_eq = false;
						var vl = (*it1)->get_names_left();
						vector<var> r = (*it1)->get_names_right();
						for (int i = 0; i < r.size(); i++){
							if (r[i] == v){
								found_eq = true;
								break;
							}
						}
						if(!found_eq){
							if(vl == v){
								found_eq = false;
								break;
							}
						}
						if (found_eq)break;
					}
					if(!found_eq)
					{
						(block_ics_list).erase(it);
						icb->set_ics_list(block_ics_list);
						dead_found=true;
						break;
					}
				}
				else{
					list<Icode_Stmt*>::iterator it1 = ++it; it--;
					bool found_eq;
					if(it1==block_ics_list.end()){
						found_eq=false;
					}
					for(; it1 != block_ics_list.end(); it1++){
						found_eq = false;
						var vl = (*it1)->get_names_left();
						vector<var> r = (*it1)->get_names_right();
						for (int i = 0; i < r.size(); i++){
							if (r[i] == v){
								found_eq = true;
								break;
							}
						}
						if(!found_eq){
							if(vl == v && v.name!="#"){
								found_eq = false;

								(block_ics_list).erase(it);
								icb->set_ics_list(block_ics_list);
								dead_found=true;
								break;
							}
						}
						if (found_eq)break;
					}
				}

				if(dead_found){
					break;
				}
			}
			if(dead_found){
				break;
			}
		}
		count++;
	}
	for (IC_Block * icb = start_block; icb != NULL; icb = icb->get_sequ()){
		list<Icode_Stmt *> l = icb->get_ics_list();
		for(ic_list_it it = l.begin(); it != l.end(); it ++)
		{
			opt_icode_list.push_back(*it);
		}
	}
}

void Sequence_Ast::set_global()
{

	list<Icode_Stmt *> l = sa_icode_list;
	for (ic_list_it it = l.begin(); it != l.end(); it ++)
	{
		set<var> s = (*it)->get_global_list();
		global = set_uni(global, s);
	}
}

set<var> Sequence_Ast::get_global()
{
	return global;
}
//////////////////////////////////////////////////////////////////////////////

template class Number_Ast<double>;
template class Number_Ast<int>;
