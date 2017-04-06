program:
    declaration_list procedure_definition_list
;

declaration_list:
    procedure_declaration_list
|
    variable_declaration_list
    procedure_declaration_list
|
    procedure_declaration_list
    variable_declaration_list
;

procedure_declaration_list:
    procedure_declaration
|
    procedure_declaration_list procedure_declaration
;

procedure_declaration:
    type NAME '(' argument_list ')' ';'
;

type:
    VOID
|
    INTEGER
|
    FLOAT
;

procedure_definition_list:
    procedure_definition
|
    procedure_definition_list procedure_definition
;

procedure_definition:
    NAME '(' argument_list ')'  '{' optional_variable_declaration_list statement_list return_stmt '}'
;

argument_list:

|
    argument
|
    argument_list ',' argument
;

argument:
    INTEGER NAME
|
    FLOAT NAME

optional_variable_declaration_list:

|
    variable_declaration_list
;

variable_declaration_list:
    variable_declaration
|
    variable_declaration_list variable_declaration
;

variable_declaration:
    declaration ';'
;

declaration:
    INTEGER variable_list
|
    FLOAT variable_list
;

variable_list:
    NAME
|
    variable_list ',' NAME
;

statement_list:
    
|
    statement_list statement
;

statement:
    matched_stmt
|
    unmatched_stmt
;

matched_stmt:
    assignment_statement
|
    WHILE '(' boolean_expression ')' matched_stmt
|
    do_while_statement
|
    IF '(' boolean_expression ')' matched_stmt ELSE matched_stmt
|
    '{' statement_list '}'
|
    return_stmt
|
    function_call
;   

unmatched_stmt:
    IF '(' boolean_expression ')' statement
|
    IF '(' boolean_expression ')' matched_stmt ELSE unmatched_stmt
|
    WHILE '(' boolean_expression ')' unmatched_stmt
;

do_while_statement:
    DO statement WHILE '(' boolean_expression ')' ';'
;

assignment_statement:
    variable ASSIGN arith_expression ';'
;

print_statement:
    PRINT '(' arith_expression ')' ';'
|
    PRINT '(' STRING ')' ';'

return_stmt:
    RETURN ';'
|
    RETURN arith_expression ';'
;

function_call:
    NAME '(' parameter_list ')' ';'
;

parameter_list:

|
    parameter
|
    parameter_list ',' parameter
;

parameter:
    arith_expression
;

arith_expression:
    arith_expression '+' arith_expression
|
    arith_expression '-' arith_expression
|
    arith_expression '*' arith_expression
|
    arith_expression '/' arith_expression
|
    '-' arith_expression %prec UMINUS
|
    '(' arith_expression ')'
|
    expression_term
|
    boolean_expression '?' arith_expression ':' arith_expression
|
    function_call
;


expression_term:
    variable
|
    constant
;

boolean_expression:
    boolean_expression AND boolean_expression
|
    boolean_expression OR boolean_expression
|   
    NOT boolean_expression
|
    '(' boolean_expression ')'
|
    relational_expression
;

relational_expression:
    arith_expression LESS_THAN arith_expression
|
    arith_expression LESS_THAN_EQUAL arith_expression
|
    arith_expression GREATER_THAN arith_expression
|   
    arith_expression GREATER_THAN_EQUAL arith_expression
|
    arith_expression EQUAL_TO arith_expression
|
    arith_expression NOT_EQUAL_TO arith_expression
;

variable:
    NAME
;

constant:
    INTEGER_NUMBER
|
    DOUBLE_NUMBER
;