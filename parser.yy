%scanner scanner.h
%scanner-token-function d_scanner.lex()
%filenames parser
%parsefun-source parser.cc

%union 
{
    int integer_value;
    double double_value;
    std::string * string_value;
    vector<std::string> *var_list;
    vector<Symbol_Table_Entry * > *decl_entry_list;
    vector<pair< Data_Type, string> > *declaration_list;
    Symbol_Table * symbol_table;
    Sequence_Ast *sequence_Ast;
    Ast * ast;
    Procedure *proc;
    list<pair<Data_Type, string> > *arg_list;
    pair<Data_Type, string> *arg;
    Return_Ast *ret_ast;
    Print_Ast *p_ast;
    Func_Call_Ast *f_ast;
};

%left OR
%left AND
%left EQUAL_TO NOT_EQUAL_TO
%left LESS_THAN LESS_THAN_EQUAL GREATER_THAN GREATER_THAN_EQUAL 
%left '+' '-'
%left '*' '/'
%right UMINUS
%right NOT
%nonassoc '('

%token <integer_value> INTEGER_NUMBER
%token <double_value> DOUBLE_NUMBER
%token <string_value> NAME
%token VOID INTEGER 
%token FLOAT
%token DO WHILE 
%token IF ELSE 
%token ASSIGN
%token RETURN

%type <symbol_table> optional_variable_declaration_list
%type <symbol_table> variable_declaration_list
%type <decl_entry_list> variable_declaration
%type <declaration_list> declaration
%type <var_list> variable_list
%type <sequence_Ast>    statement_list
%type <ast> statement
%type <ast> assignment_statement
%type <ast> arith_expression
%type <ast> expression_term
%type <ast> do_while_statement
%type <ast> unmatched_stmt
%type <ast> matched_stmt
%type <ast> boolean_expression
%type <ast> relational_expression
%type <ast> variable
%type <ast> constant

%type <> procedure_definition_list
%type <proc> procedure_declaration
%type <string_value> func_type
%type <arg_list> argument_list
%type <arg> argument
%type <> procedure_definition
%type <f_ast> function_call
%type <p_ast> print_statement
%type <ret_ast> return_stmt

%start program

%%

program:
    declaration_list procedure_definition_list
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");

        program_object.set_procedure(current_procedure, get_line_number());
        program_object.global_list_in_proc_check();
    }
    }
;

declaration_list:
    procedure_declaration_list
    {
    if (NOT_ONLY_PARSE)
    {
        Symbol_Table * global_table = new Symbol_Table();
        program_object.set_global_table(*global_table);
    }
    }
|
    variable_declaration_list
    procedure_declaration_list
    {
    if (NOT_ONLY_PARSE)
    {
        Symbol_Table * global_table = $1;

        CHECK_INVARIANT((global_table != NULL), "Global declarations cannot be null");

        program_object.set_global_table(*global_table);
    }
    }
|
    procedure_declaration_list
    variable_declaration_list
    {
    if (NOT_ONLY_PARSE)
    {
        Symbol_Table * global_table = $2;

        CHECK_INVARIANT((global_table != NULL), "Global declarations cannot be null");

        program_object.set_global_table(*global_table);
    }
    }
;

procedure_declaration_list:
    procedure_declaration
    {

    }
|
    procedure_declaration_list procedure_declaration
    {

    }
;

procedure_declaration:
    func_type NAME '(' argument_list ')' ';'
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT(($2 != NULL), "Procedure name cannot be null");
        string proc_name = *$2;
        CHECK_INVARIANT(!program_object.variable_proc_name_check(proc_name),
                                     "Overloading of the procedure is not allowed");
        string type = *$1;
        CHECK_INVARIANT(type = "void" || type = "int" || type = "float", "Unknown type in procedure_declaration");

        Data_Type dt;
        string t = $1;
        if (t == "void")
            dt = void_data_type;
        else if (t == "int")
            dt = int_data_type;
        else
            dt = double_data_type;
        Procedure * proc = new Procedure(dt, proc_name, get_line_number());
        program_object.add_procedure(proc, get_line_number());

        list<pair<Data_Type, string> > arg_list = *$4;

        if(arg_list.size() >= 2)
        {
            for(list<pair<Data_Type, string> >::iterator it = arg_list.begin(); it != arg_list.end(); it++)
            {
                for(list<pair<Data_Type, string> >::iterator it1 = ++it, it--; it1 != arg_list.end(); it1++)
                {
                    CHECK_INVARIANT(it1->second != it->second, "Identical names of formal parameters");
                }
            }
        }

        pair<Data_Type, string> p = new pair<Data_Type, string>(dt, t);
        CHECK_INVARIANT(arg_list.find(p) == arg_list.end(), "Function name used in formal parameter list");

        proc->set_argument_list(arg_list);

        Symbol_Table * loc_tab = new Symbol_Table();
        for (list<pair<Data_Type, string> >::iterator it = arg_list.begin(); it != arg_list.end(); it ++)
        {
            Symbol_Table_Entry * sym_tab_entry = new Symbol_Table_Entry(it->second, it->first, get_line_number());
            loc_tab->push_symbol(sym_tab_entry);
        }

        proc->set_local_list(arg_list);

        $$ = proc;
    }
    }
;

func_type:
    VOID
    {
    if (NOT_ONLY_PARSE)
    {
        $$ = "void";
    }
    }
|
    INTEGER
    {
    if (NOT_ONLY_PARSE)
    {
        $$ = "int";
    }
}
|
    FLOAT
    {
    if (NOT_ONLY_PARSE)
    {
        $$ = "float";
    }
    }
;

argument_list:
    // Empty argument list
    {
    if (NOT_ONLY_PARSE)
    {
        list<pair<Data_Type, string> > * arg_list = new list<pair<Data_Type, string> >();
        $$ = arg_list;
    }
    }
|
    argument ',' argument_list
    {
    if (NOT_ONLY_PARSE)
    {
        list<pair<Data_Type, string> > * arg_list = $3;
        arg_list->push_back(*$1);
        $$ = arg_list;
    }
    }
;

argument:
    INTEGER NAME
    {
    if (NOT_ONLY_PARSE)
    {
        Data_Type dt = int_data_type;
        string name = *$2;
        pair<Data_Type, string> * p = new pair<Data_Type, string>(dt, name);
        $$ = p;
    }
    }
|
    FLOAT NAME
    {
    if (NOT_ONLY_PARSE)
    {
        Data_Type dt = double_data_type;
        string name = *$2;
        pair<Data_Type, string> * p = new pair<Data_Type, string>(dt, name);
        $$ = p;
    }
    }
;

procedure_definition_list:
    procedure_definition
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT($1 != NULL, "procedure_definition can not be null");
    }
    }
|
    procedure_definition_list procedure_definition
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT($2 != NULL, "procedure_definition can not be null");
    }
    }
;

procedure_definition:
    NAME '(' argument_list ')' 
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT(($1 != NULL), "Procedure name cannot be null");
        string proc_name = *$1;
        CHECK_INVARIANT(program_object.variable_proc_name_check(proc_name), 
                        "Procedure corresponding to the name is not found");

        CHECK_INPUT ((program_object.variable_in_symbol_list_check(proc_name) == false),
            "Procedure name cannot be same as global variable", get_line_number());

        current_procedure = get_procedure(proc_name);

        list<pair<Data_Type, string> > * arg_list = $3;
        CHECK_INVARIANT(current_procedure.same_arguments(*arg_list), 
            "Formal Parameters of procedure definition and declaration do not match");
    }
    }

    '{' optional_variable_declaration_list
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");
    }
    }

    statement_list return_stmt '}'
    {
    if (NOT_ONLY_PARSE)
    {
        Sequence_Ast* seq = $8;

        CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");
        CHECK_INVARIANT((seq != NULL), "statement list cannot be null");

        current_procedure->set_sequence_ast(*seq);
    }
    }
;

optional_variable_declaration_list:
    {
    if (NOT_ONLY_PARSE)
    {
        $$ = NULL;
    }
    }
|
    variable_declaration_list
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT(($1 != NULL), "Declaration statement list cannot be null here");

        $$ = $1;
    }
    }
;

variable_declaration_list:
    variable_declaration
    {
    if (NOT_ONLY_PARSE)
    {
        // if declaration is local then no need to check in global list
        // if declaration is global then this list is global list

        Symbol_Table * decl_list = new Symbol_Table();

        CHECK_INVARIANT(($1 != NULL), "The declaration statement cannot be null");

        vector<Symbol_Table_Entry * > *decl_entry_list = $1;

        for(vector<Symbol_Table_Entry * >::iterator it = decl_entry_list->begin(); it != decl_entry_list->end(); ++it)
        {
            string decl_name = (*it) -> get_variable_name();
            CHECK_INPUT((program_object.variable_proc_name_check(decl_name) == false),
                "Procedure name cannot be same as the variable name", get_line_number());
            if(current_procedure != NULL)
            {
                CHECK_INPUT((current_procedure->get_proc_name() != decl_name),
                    "Variable name cannot be same as procedure name", get_line_number());
                CHECK_INVARIANT(current_procedure->variable_in_symbol_list_check(), 
                    "Variable already declared in procedure");
                current_procedure->add_symbol_entry(*it);
            }

            CHECK_INPUT((decl_list->variable_in_symbol_list_check(decl_name) == false), 
                    "Variable is declared twice", get_line_number());

            decl_list->push_symbol(*it);
        }
        $$ = decl_list;
    }
    }
|
    variable_declaration_list variable_declaration
    {
    if (NOT_ONLY_PARSE)
    {
        // if declaration is local then no need to check in global list
        // if declaration is global then this list is global list

        Symbol_Table * decl_list = $1;

        CHECK_INVARIANT(($2 != NULL), "The declaration statement cannot be null");
        CHECK_INVARIANT((decl_list != NULL), "The declaration statement list cannot be null");

        vector<Symbol_Table_Entry * > *decl_entry_list = $2;

        for(vector<Symbol_Table_Entry * >::iterator it = decl_entry_list->begin(); it != decl_entry_list->end(); ++it)
        {

            string decl_name = (*it) -> get_variable_name();
            CHECK_INPUT((program_object.variable_proc_name_check(decl_name) == false),
                "Procedure name cannot be same as the variable name", get_line_number());
            if(current_procedure != NULL)
            {
                CHECK_INPUT((current_procedure->get_proc_name() != decl_name),
                    "Variable name cannot be same as procedure name", get_line_number());
                CHECK_INVARIANT(current_procedure->variable_in_symbol_list_check(), 
                    "Variable already declared in procedure");
                current_procedure->add_symbol_entry(*it);
            }

            CHECK_INPUT((decl_list->variable_in_symbol_list_check(decl_name) == false), 
                    "Variable is declared twice", get_line_number());

            decl_list->push_symbol(*it);
        }
        $$ = decl_list;
    }
    }
;

variable_declaration:
    declaration ';'
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT(($1 != NULL), "Declaration cannot be null");
        vector<pair< Data_Type, string> > *declaration_list = $1;
        vector<Symbol_Table_Entry * > *decl_entry_list = new vector<Symbol_Table_Entry *>();
        for(vector<pair< Data_Type, string> >::iterator it = declaration_list->begin(); it!=declaration_list->end(); ++it)
        {
            Data_Type type = it->first;
            string decl_name = it->second;
            Symbol_Table_Entry * decl_entry = new Symbol_Table_Entry(decl_name, type, get_line_number());
            decl_entry_list -> push_back(decl_entry);
        }

        $$ = decl_entry_list;

    }
    }
;

declaration:
    INTEGER variable_list
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT(($2!=NULL), "variable list cannot be null");
        vector<std:: string> *var_list = $2;
        Data_Type type = int_data_type;
        vector<pair< Data_Type, string> > *declaration_list = new vector< pair<Data_Type, string> >();
        for(vector<string>::iterator it=var_list->begin(); it!=var_list->end(); ++it){
            string name = *it;
            pair< Data_Type, string> *declaration = new pair< Data_Type, string>(type, name);
            declaration_list -> push_back(*declaration);
        }
        $$ = declaration_list;
    }
    }
|
    FLOAT variable_list
    {
    if(NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT(($2!=NULL), "variable list cannot be null");
        vector<std:: string> *var_list = $2;
        Data_Type type = double_data_type;
        vector<pair< Data_Type, string> > *declaration_list = new vector< pair<Data_Type, string> >();
        for(vector<string>::iterator it=var_list->begin(); it!=var_list->end(); ++it){
            string name = *it;
            pair< Data_Type, string> *declaration = new pair< Data_Type, string>(type, name);
            declaration_list -> push_back(*declaration);
        }
        $$ = declaration_list;
    }
    }
;

variable_list:
    NAME
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT(($1!=NULL),"Name cannot be null");
        string name = *($1);
        vector<std:: string> *var_list = new vector<std:: string>();
        var_list->push_back(name);
        $$ = var_list;
    }
    }
|
    variable_list ',' NAME
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT((($1!=NULL)&&($3!=NULL)), "List cannot be null and terminal name cannot be null");
        vector<std:: string> *var_list = $1;
        string name = *($3);
        var_list->push_back(name);
        $$ = (var_list);
    }
    }
;

statement_list:
    {
    if (NOT_ONLY_PARSE)
    {
        //ADD CODE HERE
        Sequence_Ast *assign_list = new Sequence_Ast(get_line_number());
        $$ = assign_list;
    }
    }
|
    statement_list statement
    {
    if (NOT_ONLY_PARSE)
    {
        //ADD CODE HERE
        Sequence_Ast *assign_list = $1;
        CHECK_INVARIANT(($2!=NULL), "non terminal assignment_statement cannot be null");
        Ast *assgn_stmt = $2;
        assign_list->ast_push_back(assgn_stmt);
        $$ = assign_list;
    }
    }
;

statement:
	matched_stmt
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1!=NULL), "matched statement cannot be null");
		Ast *matched_stmt = $1;
		$$ = matched_stmt;
	}
	}
|
	unmatched_stmt
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1!=NULL), "unmatched statement cannot be null");
		Ast *unmatched_stmt = $1;
		$$ = unmatched_stmt;
	}
	}
;

matched_stmt:
    assignment_statement
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT(($1 != NULL), "Statement can not be null");
        Ast * asgn_stmt = $1;
        $$ = asgn_stmt;
    }
    }
|
    WHILE '(' boolean_expression ')' matched_stmt
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT(($3!=NULL && $5 != NULL), "Boolean expression cannot be null");
        Ast * cond = $3;
        Ast * body = $5;
        Iteration_Statement_Ast *while_ast = new Iteration_Statement_Ast(cond, body, get_line_number(), false);
        $$ = while_ast;
    }
    }
|
    do_while_statement
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT(($1 != NULL), "Statement can not be null");
        Ast * do_while_stmt = $1;
        $$ = do_while_stmt;
    }
    }
|
	IF '(' boolean_expression ')' matched_stmt ELSE matched_stmt
	{
	if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT(($3!=NULL && $5 != NULL && $7 != NULL), "Boolean expression cannot be null");
        Ast * cond = $3;
        Ast * then_part  = $5;
        Ast * else_part = $7;
        Selection_Statement_Ast *matched_ast = new Selection_Statement_Ast(cond, then_part, else_part, get_line_number());
        $$ = matched_ast;
    }
	}
|
	'{' statement_list '}'
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2!=NULL), "statement list cannot be null");
		Ast *statement_list = $2;
		$$ = statement_list;
	}	
	}
|
    return_stmt
    {
    if(NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT($1!=NULL, "Return statement cannot be null", get_line_number());
        Ast *ret_stmt = $1;
        $$ = ret_stmt;
    }
    }
|
    function_call
    {
    if(NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT($1!=NULL, "Function call cannot be null", get_line_number());
        Ast *func_ast = $1;
        $$ = func_ast;
    }
    }
;   

unmatched_stmt:
	IF '(' boolean_expression ')' statement
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($3!=NULL)&&($5!=NULL), "Boolean expression and statement cannot be null");
		Ast *cond = $3;
		Ast *then_part = $5;
		Ast *else_part = new Sequence_Ast(get_line_number());
		Selection_Statement_Ast *if_ast = new Selection_Statement_Ast(cond, then_part, else_part, get_line_number());
		$$ = if_ast;
	}
	}
|
	IF '(' boolean_expression ')' matched_stmt ELSE unmatched_stmt
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($3!=NULL && $5 != NULL && $7 != NULL), "Boolean expression cannot be null");
        Ast * cond = $3;
        Ast * then_part  = $5;
        Ast * else_part = $7;
        Selection_Statement_Ast *unmatched_ast = new Selection_Statement_Ast(cond, then_part, else_part, get_line_number());
        $$ = unmatched_ast;
	}
	}
|
	WHILE '(' boolean_expression ')' unmatched_stmt
	{
	if (NOT_ONLY_PARSE)
	{
	    CHECK_INVARIANT(($3!=NULL && $5 != NULL), "Boolean expression cannot be null");
	    Ast * cond = $3;
	    Ast * body = $5;
	    Iteration_Statement_Ast *while_ast = new Iteration_Statement_Ast(cond, body, get_line_number(), false);
	    $$ = while_ast;
	}
	}
;

do_while_statement:
    DO statement WHILE '(' boolean_expression ')' ';'
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT(($5!=NULL && $2 != NULL), "Boolean expression cannot be null");
        Ast *cond = $5;
        Ast * body = $2;
        // cout<<body<<endl;
        Iteration_Statement_Ast *do_while_ast = new Iteration_Statement_Ast(cond, body, get_line_number(), true);;
        $$ = do_while_ast;
    }
    }
;

assignment_statement:
    variable ASSIGN arith_expression ';'
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT((($1!=NULL)&&($3!=NULL)), "lhs/rhs cannot be null");
        Ast *variable = $1;
        Ast *arith_expression = $3;
        Ast *assgn_stmt = new Assignment_Ast(variable, arith_expression, get_line_number());
        $$ = assgn_stmt;
    }
    }
;

print_statement:
    PRINT '(' arith_expression ')' ';'
    {
    if(NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT($3!=NULL, "arith_expression cannot be null");
        Ast * p_ast = $3;
        Print_Ast * print_ast = new Print_Ast(p_ast, get_line_number());
        Data_Type dt = void_data_type;
        print_ast->set_data_type(dt);
        $$ = print_ast;
    }
    }
|
    PRINT '(' STRING ')' ';'
    {
    if(NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT($3!=NULL, "string cannot be null");
        string s = *$3;
        Print_Ast *p_ast = new Print_Ast(s, get_line_number());
        Data_Type dt = void_data_type;
        p_ast->set_data_type(dt);
        $$ = p_ast;
    }
    }
;

return_stmt:
    RETURN ';'
    {
    if(NOT_ONLY_PARSE)
    {
        Return_Ast *ret_ast = new Return_Ast(NULL, get_line_number());
        Data_Type dt = void_data_type;
        ret_ast->set_data_type(dt);
        $$ = ret_ast;
    }
    }
|
    RETURN arith_expression ';'
    {
    if(NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT($2!=NULL, "return argument cannot be null ", get_line_number());
        Ast *ret = $2;
        Return_Ast * ret_ast = new Return_Ast(ret, get_line_number());
        ret_ast->set_data_type(ret->get_data_type());
        $$ = ret_ast;
    }
    }
;

function_call:
    NAME '(' parameter_list ')' ';'
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT(($1)!=NULL, "Function name in function call cannot be null")
        string name = *($1);
        if(current_procedure->variable_in_symbol_list_check(name)){
            CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Variable name and function name matches", get_line_number());
        }
        else{
            if(!program_object.variable_proc_name_check(name)){
                CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Function has not been declared", get_line_number());
            }
        }
        list<Ast * > arg_list = $3;
        Procedure *proc= program_object.get_procedure(name);
        list<Data_Type> type_list;
        list<Ast *>::iterator =it;
        for(it=arg_list.begin();it!=arg_list.end();it++)
        {
            type_list.push_back((*it)->get_data_type());
        }
        if(!proc->argument_type_check(type_list)){
            CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Arguments do not match the function definition", get_line_number());
        }
        Func_Call_Ast *func_ast = new Func_Call_Ast(name, arg_list, get_line_number());
        func_ast->set_data_type(proc->get_return_type());
        $$ = func_ast;
    }
    }
;

parameter_list:
    {
    if(NOT_ONLY_PARSE)
    {
        list<Ast *> arg_list;
        $$ = arg_list;
    }
    }
|
    parameter_list ',' parameter
    {
    if(NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT($1!=NULL && $3!=NULL, "parameter cannot be null", get_line_number());
        list<Ast *> arg_list = $1;
        Ast *param = $3;
        arg_list.push_back(param);
        $$ = arg_list;
    }
    }
;

parameter:
    arith_expression
    {
    if(NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT($1!=NULL, "arith_expression cannot be null", get_line_number());
        Ast * arith_ast = $1;
        $$ = arith_ast;
    }
    }
;

arith_expression:
    arith_expression '+' arith_expression
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "Operands of arithmetic expression can not be null");
        Ast * lhs = $1;
        Ast * rhs = $3;
        Ast * plus = new Plus_Ast(lhs, rhs, get_line_number());
        $$ = plus;
    }
    }
|
    arith_expression '-' arith_expression
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "Operands of arithmetic expression can not be null");
        Ast * lhs = $1;
        Ast * rhs = $3;
        Ast * minus = new Minus_Ast(lhs, rhs, get_line_number());
        $$ = minus;
    }
    }
|
    arith_expression '*' arith_expression
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "Operands of arithmetic expression can not be null");
        Ast * lhs = $1;
        Ast * rhs = $3;
        Ast * mult = new Mult_Ast(lhs, rhs, get_line_number());
        $$ = mult;
    }
    }
|
    arith_expression '/' arith_expression
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "Operands of arithmetic expression can not be null");
        Ast * lhs = $1;
        Ast * rhs = $3;
        Ast * divide = new Divide_Ast(lhs, rhs, get_line_number());
        $$ = divide;
    }
    }
|
    '-' arith_expression %prec UMINUS
    {
    if(NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT(($2!=NULL), "Operand of unary minus cannot be null");
        Ast *operand = $2;
        Ast *uminus_ast = new UMinus_Ast( operand, NULL, get_line_number());
        $$ = uminus_ast;
    }
    }
|
    '(' arith_expression ')'
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT(($2 != NULL), "Expression in Paranthesis can not be null");
        Ast * expr = $2;
        $$ = expr;
    }
    }
|
    expression_term
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT(($1 != NULL), "Term in an expression can not be null");
        Ast * term = $1;
        $$ = term;
    }
    }
|
    boolean_expression '?' arith_expression ':' arith_expression
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT((($1 != NULL) && $3 != NULL && $5 != NULL), "Operands of conditional operator ?: can not be null");
        Ast * condition = $1;
        Ast * lhs = $3;
        Ast * rhs = $5;
        Ast * cond_expr = new Conditional_Operator_Ast(condition, lhs, rhs, get_line_number());
        $$ = cond_expr;
    }
    }
|
    function_call
    {
    if(NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT(($1!=NULL), "Func call cannot be null", get_line_number());
        Ast *func_ast = $1;
        $$ = func_ast;
    }
    }
;


expression_term:
    variable
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT(($1!=NULL), "variable cannot be null");
        Ast *ast = $1;
        $$ = ast;
    }
    }
|
    constant
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT(($1!=NULL), "constant cannot be null");
        Ast *ast = $1;
        $$ = ast;
    }
    }
;

boolean_expression:
    boolean_expression AND boolean_expression
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "Operands of a boolean AND expression can not be null");
        Ast * lhs = $1;
        Ast * rhs = $3;
        Boolean_Op bop = boolean_and;
        Ast * and_stmt = new Boolean_Expr_Ast(lhs, bop, rhs, get_line_number());
        $$ = and_stmt;
    }
    }
|
    boolean_expression OR boolean_expression
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "Operands of a boolean OR expression can not be null");
        Ast * lhs = $1;
        Ast * rhs = $3;
        Boolean_Op bop = boolean_or;
        Ast * or_stmt = new Boolean_Expr_Ast(lhs, bop, rhs, get_line_number());
        $$ = or_stmt;
    }
    }
|   
    NOT boolean_expression
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT(($2 != NULL), "Operands of a boolean NOT expression can not be null");
        Ast * lhs = NULL;
        Ast * rhs = $2;
        Boolean_Op bop = boolean_not;
        Ast * not_stmt = new Boolean_Expr_Ast(lhs, bop, rhs, get_line_number());
        $$ = not_stmt;
    }
    }
|
    '(' boolean_expression ')'
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT(($2 != NULL), "Boolean expression in Paranthesis can not be null");
        Ast * expr = $2;
        $$ = expr;
    }
    }
|
    relational_expression
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT(($1 != NULL), "Boolean expression is not Valid");
        Ast * expr = $1;
        $$ = expr;
    }
    }
;

relational_expression:
    arith_expression LESS_THAN arith_expression
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "Relational Expression operands can not be null");
        Ast * lhs = $1;
        Ast * rhs = $3;
        Relational_Op rop = less_than;
        Ast * lt = new Relational_Expr_Ast(lhs, rop, rhs, get_line_number());
        $$ = lt;
    }
    }
|
    arith_expression LESS_THAN_EQUAL arith_expression
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "Relational Expression operands can not be null");
        Ast * lhs = $1;
        Ast * rhs = $3;
        Relational_Op rop = less_equalto;
        Ast * lte = new Relational_Expr_Ast(lhs, rop, rhs, get_line_number());
        $$ = lte;
    }
    }
|
    arith_expression GREATER_THAN arith_expression
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "Relational Expression operands can not be null");
        Ast * lhs = $1;
        Ast * rhs = $3;
        Relational_Op rop = greater_than;
        Ast * gt = new Relational_Expr_Ast(lhs, rop, rhs, get_line_number());
        $$ = gt;
    }
    }
|   
    arith_expression GREATER_THAN_EQUAL arith_expression
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "Relational Expression operands can not be null");
        Ast * lhs = $1;
        Ast * rhs = $3;
        Relational_Op rop = greater_equalto;
        Ast * gte = new Relational_Expr_Ast(lhs, rop, rhs, get_line_number());
        $$ = gte;
    }
    }
|
    arith_expression EQUAL_TO arith_expression
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "Relational Expression operands can not be null");
        Ast * lhs = $1;
        Ast * rhs = $3;
        Relational_Op rop = equalto;
        Ast * eq = new Relational_Expr_Ast(lhs, rop, rhs, get_line_number());
        $$ = eq;
    }
    }
|
    arith_expression NOT_EQUAL_TO arith_expression
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "Relational Expression operands can not be null");
        Ast * lhs = $1;
        Ast * rhs = $3;
        Relational_Op rop = not_equalto;
        Ast * neq = new Relational_Expr_Ast(lhs, rop, rhs, get_line_number());
        $$ = neq;
    }
    }
;

variable:
    NAME
    {
    if (NOT_ONLY_PARSE)
    {
        Symbol_Table_Entry * var_table_entry;

        CHECK_INVARIANT(($1 != NULL), "Variable name cannot be null");

        if (current_procedure->variable_in_symbol_list_check(*$1))
             var_table_entry = &(current_procedure->get_symbol_table_entry(*$1));

        else if (program_object.variable_in_symbol_list_check(*$1))
            var_table_entry = &(program_object.get_symbol_table_entry(*$1));

        else
            CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Variable has not been declared", get_line_number());

        $$ = new Name_Ast(*$1, *var_table_entry, get_line_number());

        delete $1;
    }
    }
;

constant:
    INTEGER_NUMBER
        {
        if (NOT_ONLY_PARSE)
        {
            CHECK_INVARIANT((&($1)!=NULL), "Constant number cannot be null");
            int number = $1;
            Data_Type type = int_data_type;
            Ast *number_ast = new Number_Ast<int>(number, type, get_line_number());
            $$ = number_ast;
        }
        }
|
    DOUBLE_NUMBER
    {
    if (NOT_ONLY_PARSE)
    {
        CHECK_INVARIANT((&($1)!=NULL), "Constant number cannot be null");
        double number = $1;
        Data_Type type = double_data_type;
        Ast *number_ast = new Number_Ast<double>(number, type, get_line_number());
        $$ = number_ast;
    }
    }
;