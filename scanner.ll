%filenames="scanner"
%lex-source="scanner.cc"

digit [0-9]
letter [A-Za-z]
brackets [(){}]
underscore [_]
float1 ({digit}+)\.({digit}+)?([eE][\-+]?{digit}+)?
float2 \.{digit}+([eE][\-+]?{digit}+)?
float3 ({digit}+)[eE][\-+]?{digit}+

%%
//ADD YOUR CODE HERE
[+*\-/] {
	cout << "Found an operator whose lexeme is '" << matched() << "'\n";
	store_token_name("ARITHOP");
	return matched()[0];
}

(void) {
	cout << "Found void whose lexeme is '" << matched() << "'\n";
	store_token_name("VOID");
	return Parser::VOID;
}

(int) {
	cout << "Found type whose lexeme is '" << matched() << "'\n";
	store_token_name("INTEGER");
	return Parser::INTEGER;
}

(float) {
	cout << "Found type whose lexeme is '" << matched() << "'\n";
	store_token_name("FLOAT");
	return Parser::FLOAT;
}

(do) {
	cout << "Found type whose lexeme is '" << matched() << "'\n";
	store_token_name("DO");
	return Parser::DO;
}

(while) {
	cout << "Found type whose lexeme is '" << matched() << "'\n";
	store_token_name("WHILE");
	return Parser::WHILE;
}

(if) {
	cout << "Found type whose lexeme is '" << matched() << "'\n";
	store_token_name("IF");
	return Parser::IF;
}

(else) {
	cout << "Found type whose lexeme is '" << matched() << "'\n";
	store_token_name("ELSE");
	return Parser::ELSE;
}

(return) {
	cout << "Found type whose lexeme is '" << matched() << "'\n";
	store_token_name("RETURN");
	return Parser::RETURN;
}

(print) {
	cout << "Found type whose lexeme is '" << matched() << "'\n";
	store_token_name("PRINT");
	return Parser::PRINT;
}



({float1}|{float2}|{float3}) {
	cout << "Found a float whose lexeme is '" << matched() << "'\n";
	ParserBase::STYPE__ *val = getSval();	
  	val->double_value = atof(matched().c_str());
  	store_token_name("FNUM");
	return Parser::DOUBLE_NUMBER; 
}

{digit}+ {
	cout << "Found a number whose lexeme is '" << matched() << "'\n";
	ParserBase::STYPE__ *val = getSval();	
  	val->integer_value = atoi(matched().c_str());
  	store_token_name("NUM");
	return Parser::INTEGER_NUMBER; 
}

({underscore}|{letter})({letter}|{digit}|{underscore})* {
	cout << "Found a identifier whose lexeme is '" << matched() << "'\n";
	ParserBase::STYPE__ *val = getSval();	
  	val->string_value = new std::string(matched());
  	store_token_name("NAME");
	return Parser::NAME;
}

[=] {
	cout << "Found an assignment operator whose lexeme is '" << matched() << "'\n";
	store_token_name("ASSIGN_OP");
	return Parser::ASSIGN;
}

[<] {
	cout << "Found an assignment operator whose lexeme is '" << matched() << "'\n";
	store_token_name("LESS_THAN_OP");
	return Parser::LESS_THAN;
}

[>] {
	cout << "Found an assignment operator whose lexeme is '" << matched() << "'\n";
	store_token_name("GREATER_THAN_OP");
	return Parser::GREATER_THAN;
}

[=][=] {
	cout << "Found an assignment operator whose lexeme is '" << matched() << "'\n";
	store_token_name("EQUAL_TO");
	return Parser::EQUAL_TO;
}

[!][=] {
	cout << "Found an assignment operator whose lexeme is '" << matched() << "'\n";
	store_token_name("NOT_EQUAL_TO");
	return Parser::NOT_EQUAL_TO;
}

[>][=] {
	cout << "Found an assignment operator whose lexeme is '" << matched() << "'\n";
	store_token_name("GREATER_EQUAL_TO");
	return Parser::GREATER_THAN_EQUAL;
}

[<][=] {
	cout << "Found an assignment operator whose lexeme is '" << matched() << "'\n";
	store_token_name("LESS_EQUAL_TO");
	return Parser::LESS_THAN_EQUAL;
}

[!] {
	cout << "Found an assignment operator whose lexeme is '" << matched() << "'\n";
	store_token_name("NOT");
	return Parser::NOT;
}

[|][|] {
	cout << "Found an assignment operator whose lexeme is '" << matched() << "'\n";
	store_token_name("OR");
	return Parser::OR;
}

[&][&] {
	cout << "Found an assignment operator whose lexeme is '" << matched() << "'\n";
	store_token_name("AND");
	return Parser::AND;
}

[:?,] {
	cout << "Found an assignment operator whose lexeme is '" << matched() << "'\n";
	return matched()[0];
}


[;] {
	cout << "Found a semi-colon whose lexeme is '" << matched() << "'\n";
	store_token_name("META CHAR");
	return matched()[0];
}

(\"(\\.|[^"])*\") {
	cout << "Found an string whose lexeme is '" << matched() << "'\n";
	ParserBase::STYPE__ *val = getSval();	
  	val->string_value = new std::string(matched());
	store_token_name("STRING");
	return Parser::STRING;
}

{brackets} {
	cout << "Found a bracket whose lexeme is '" << matched() << "'\n";
	store_token_name("META CHAR");
	return matched()[0];
}
\n    		|
";;".*  	|
[ \t]*";;".*	|
[ \t]*"//".*	|
[ \t]		{
			if (command_options.is_show_tokens_selected())
				ignore_token();
		}

.		{ 
			string error_message;
			error_message =  "Illegal character `" + matched();
			error_message += "' on line " + lineNr();
			
			CHECK_INPUT(CONTROL_SHOULD_NOT_REACH, error_message, lineNr());
		}
