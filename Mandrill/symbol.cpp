// symbol.cpp

#include <cstdlib>
#include <cassert>
#include "parse.hpp"
#include "lexer.hpp"
#include "errors.hpp"

// print the base types...
std::ostream& operator<<( std::ostream& os, const Type::BaseType& t )
{
	switch( t )
	{
		case Type::INT: os << "INT"; break;
		case Type::REAL: os << "REAL"; break;
		case Type::CHAR: os << "CHAR"; break;
		case Type::BOOL: os << "BOOL"; break;
	}

	return os;
}

// This function gets the type of a subtree pointed at by tree
// tree must point to the <symbol decl> node !!!!
Type getType( Parser::ParseTree* tree )
{
	// Shouldn't really happen
	if( tree->node.is_leaf )
		throw Error( "Type Deduction Error" );
	if( tree->node.non_term != NonTerminals::SYMBOL_DECL )
		throw Error( "Type Deduction Error 2" );

	Type type;
	Parser::ParseTree* base_t = tree->children[2]->children[0]->children[0];

	switch( base_t->node.token.token_type )
	{
	case TokenTypes::KW_INT:
		type.base = Type::INT;
		break;

	case TokenTypes::KW_REAL:
		type.base = Type::REAL;
		break;

	case TokenTypes::KW_BOOL:
		type.base = Type::BOOL;
		break;

	case TokenTypes::KW_CHAR:
		type.base = Type::CHAR;
		break;

	default:
		throw Error( "Type could not be figured!" );
	}

	// Now we must do the array part!
	Parser::ParseTree* curr = tree->children[2]->children[1];
	type.array_dimension = 0;

	while( curr->children.size( ) > 1 )
	{
		type.array_dimension++;
		type.array_sizes.push_back( atoi( curr->children[1]->node.token.lexeme.c_str( ) ) );
		
		curr = curr->children[3];
	}

	// override in function adding
	type.function = false;

	return type;
}


// Adds type and id information to the symbol table given the parse tree
// This is just for identifiers that are declared here - locals and params!
void addTypes( SymbolTable& table, Parser::ParseTree* tree )
{
	if( tree->node.is_leaf )
		return;

	// A symbol declaration
	if( tree->node.non_term == NonTerminals::SYMBOL_DECL )
	{
		SymbolEntry entry;
		entry.lexeme = tree->children[0]->node.token.lexeme;
		entry.line_number = tree->children[0]->node.token.line_number;
		
		entry.type = getType( tree );

		// Now we have the full type of entry!
		std::pair<std::string, SymbolEntry> new_entry;
		new_entry.first = entry.lexeme;
		new_entry.second = entry;

		// check for multiple definitions...
		if( table.count( entry.lexeme ) > 0 )
		{
			std::string message = "Symbol ";
			message += entry.lexeme;
			throw Error( message + " already defined", entry.line_number );
		}

		table.insert( new_entry );
	}

	// recurse
	for( unsigned int i=0; i<tree->children.size( ); i++ )
	{
		addTypes( table, tree->children[i] );
	}
}




// this function adds a function to a symbol table (with types incl. params)
void addFunctionToTable( SymbolTable& func_table, Parser::ParseTree* tree )
{
	// Only do functions!	... *should* never happen
	if( tree->node.is_leaf )
		return;
	if( tree->node.non_term != NonTerminals::FUNCTION )
		return;

	// getting the  name  and base type is easy
	SymbolEntry entry;
	entry.lexeme = tree->children[0]->children[0]->node.token.lexeme;
	entry.line_number = tree->children[0]->children[0]->node.token.line_number;
	
	entry.type = getType( tree->children[0] );


	// Now we must figure the parameters
	// First we do the first parameter!
	if( !tree->children[2]->children[0]->node.is_leaf )
	{
		// there are parameters!

		// the first parameter
		entry.type.parameters.push_back( getType( tree->children[2]->children[0] ) );

		entry.type.param_names.push_back( tree->children[2]->children[0]->children[0]->node.token.lexeme );
		
		Parser::ParseTree* curr = tree->children[2]->children[1];
		while( curr->children.size( ) > 1 )
		{
			// Get the type of the next parameter
			entry.type.parameters.push_back( getType( curr->children[1] ) );

			entry.type.param_names.push_back( curr->children[1]->children[0]->node.token.lexeme );

			// Move to the next one after that...
			curr = curr->children[2];
		}
	}

	entry.type.function = true;

	// check for multiple definitions...
	if( func_table.count( entry.lexeme ) > 0 )
	{
		std::string message = "Symbol ";
		message += entry.lexeme;
		throw Error( message + " already defined", entry.line_number );
	}

	// Now we have the full type of entry!
	std::pair<std::string, SymbolEntry> new_entry;
	new_entry.first = entry.lexeme;
	new_entry.second = entry;
	func_table.insert( new_entry );
}


void addStdLib( SymbolTable& func_table )
{
	// I will do more later
	{
		SymbolEntry entry;
		std::pair<std::string, SymbolEntry> new_entry;
		
		entry.lexeme = "printInt";
		entry.type.base = Type::INT;
		entry.type.array_dimension = 0;
		entry.type.function = true;

		Type param;
		param.base = Type::INT;
		param.array_dimension = 0;
		entry.type.parameters.push_back( param );

		// Now we have the full type of entry!
		new_entry.first = entry.lexeme;
		new_entry.second = entry;
		func_table.insert( new_entry );
	}
	{
		SymbolEntry entry;
		std::pair<std::string, SymbolEntry> new_entry;

		entry.lexeme = "newLine";
		entry.type.base = Type::INT;
		entry.type.array_dimension = 0;
		entry.type.function = true;

		// Now we have the full type of entry!
		new_entry.first = entry.lexeme;
		new_entry.second = entry;
		func_table.insert( new_entry );
	}
	{
		SymbolEntry entry;
		std::pair<std::string, SymbolEntry> new_entry;
		
		entry.lexeme = "printChar";
		entry.type.base = Type::CHAR;
		entry.type.array_dimension = 0;
		entry.type.function = true;

		Type param;
		param.base = Type::CHAR;
		param.array_dimension = 0;
		entry.type.parameters.push_back( param );

		// Now we have the full type of entry!
		new_entry.first = entry.lexeme;
		new_entry.second = entry;
		func_table.insert( new_entry );
	}
	{
		SymbolEntry entry;
		std::pair<std::string, SymbolEntry> new_entry;
		
		entry.lexeme = "intOfChar";
		entry.type.base = Type::INT;
		entry.type.array_dimension = 0;
		entry.type.function = true;

		Type param;
		param.base = Type::CHAR;
		param.array_dimension = 0;
		entry.type.parameters.push_back( param );

		// Now we have the full type of entry!
		new_entry.first = entry.lexeme;
		new_entry.second = entry;
		func_table.insert( new_entry );
	}
	{
		SymbolEntry entry;
		std::pair<std::string, SymbolEntry> new_entry;
		
		entry.lexeme = "charOfInt";
		entry.type.base = Type::CHAR;
		entry.type.array_dimension = 0;
		entry.type.function = true;

		Type param;
		param.base = Type::INT;
		param.array_dimension = 0;
		entry.type.parameters.push_back( param );

		// Now we have the full type of entry!
		new_entry.first = entry.lexeme;
		new_entry.second = entry;
		func_table.insert( new_entry );
	}
	{
		SymbolEntry entry;
		std::pair<std::string, SymbolEntry> new_entry;
		
		entry.lexeme = "realOfInt";
		entry.type.base = Type::REAL;
		entry.type.array_dimension = 0;
		entry.type.function = true;

		Type param;
		param.base = Type::INT;
		param.array_dimension = 0;
		entry.type.parameters.push_back( param );

		// Now we have the full type of entry!
		new_entry.first = entry.lexeme;
		new_entry.second = entry;
		func_table.insert( new_entry );
	}
	{
		SymbolEntry entry;
		std::pair<std::string, SymbolEntry> new_entry;
		
		entry.lexeme = "intOfReal";
		entry.type.base = Type::INT;
		entry.type.array_dimension = 0;
		entry.type.function = true;

		Type param;
		param.base = Type::REAL;
		param.array_dimension = 0;
		entry.type.parameters.push_back( param );

		// Now we have the full type of entry!
		new_entry.first = entry.lexeme;
		new_entry.second = entry;
		func_table.insert( new_entry );
	}
	{
		SymbolEntry entry;
		std::pair<std::string, SymbolEntry> new_entry;
		
		entry.lexeme = "printReal";
		entry.type.base = Type::REAL;
		entry.type.array_dimension = 0;
		entry.type.function = true;

		Type param;
		param.base = Type::REAL;
		param.array_dimension = 0;
		entry.type.parameters.push_back( param );

		// Now we have the full type of entry!
		new_entry.first = entry.lexeme;
		new_entry.second = entry;
		func_table.insert( new_entry );
	}
	{		
		SymbolEntry entry;
		std::pair<std::string, SymbolEntry> new_entry;
		
		entry.lexeme = "readReal";
		entry.type.base = Type::REAL;
		entry.type.array_dimension = 0;
		entry.type.function = true;

		// Now we have the full type of entry!
		new_entry.first = entry.lexeme;
		new_entry.second = entry;
		func_table.insert( new_entry );
	}
	{
		SymbolEntry entry;
		std::pair<std::string, SymbolEntry> new_entry;
		
		entry.lexeme = "readInt";
		entry.type.base = Type::INT;
		entry.type.array_dimension = 0;
		entry.type.function = true;

		// Now we have the full type of entry!
		new_entry.first = entry.lexeme;
		new_entry.second = entry;
		func_table.insert( new_entry );
	}
	{
		SymbolEntry entry;
		std::pair<std::string, SymbolEntry> new_entry;
		
		entry.lexeme = "printHelloWorld";
		entry.type.base = Type::INT;
		entry.type.array_dimension = 0;
		entry.type.function = true;

		// Now we have the full type of entry!
		new_entry.first = entry.lexeme;
		new_entry.second = entry;
		func_table.insert( new_entry );
	}
	{
		SymbolEntry entry;
		std::pair<std::string, SymbolEntry> new_entry;
		
		entry.lexeme = "printBool";
		entry.type.base = Type::BOOL;
		entry.type.array_dimension = 0;
		entry.type.function = true;

		Type param;
		param.base = Type::BOOL;
		param.array_dimension = 0;
		entry.type.parameters.push_back( param );

		// Now we have the full type of entry!
		new_entry.first = entry.lexeme;
		new_entry.second = entry;
		func_table.insert( new_entry );
	}

}


void printType( const Type& type, std::ostream& os )
{
	os << "Base type: " << type.base << "\n";
	os << "Array dim: " << type.array_dimension << "\n";

	for( int i=0; i<type.array_dimension; ++i )
	{
		os << "Dim " << i << " " << type.array_sizes[i] << "\n";
	}

	if( type.function )
	{
		os << "Function Parameters:<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
		
		for( unsigned int i=0; i<type.parameters.size( ); ++i )
		{
			printType( type.parameters[i], os );
		}

		os << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>End Function Parameters\n";
	}

	os << "\n";
}


void printTable( const SymbolTable& table, std::ostream& os )
{
	for( SymbolTable::const_iterator i=table.begin( ); i!=table.end( ); ++i )
	{
		os << "Symbol:\n";
		os << "lexeme    " << i->second.lexeme << "\n";
		
		printType( i->second.type, os );
	}
}



















// Begin crazy type checking code....




// Forward declarations are so I can keep functions in logical order
// rather than backwards order...I think
Type checkOr( Parser::ParseTree* o, const SymbolTable& locals, const SymbolTable& functions );
Type checkAnd( Parser::ParseTree* a, const SymbolTable& locals, const SymbolTable& functions );
Type checkEq( Parser::ParseTree* eq, const SymbolTable& locals, const SymbolTable& functions );
Type checkRel( Parser::ParseTree* rel, const SymbolTable& locals, const SymbolTable& functions );
Type checkAdd( Parser::ParseTree* add, const SymbolTable& locals, const SymbolTable& functions );
Type checkMult( Parser::ParseTree* add, const SymbolTable& locals, const SymbolTable& functions );
Type checkPow( Parser::ParseTree* pow, const SymbolTable& locals, const SymbolTable& functions );
Type checkUnary( Parser::ParseTree* unary, const SymbolTable& locals, const SymbolTable& functions );
Type checkTerm( Parser::ParseTree* term, const SymbolTable& locals, const SymbolTable& functions );
Type checkFuncCall( Parser::ParseTree* fcall, const SymbolTable& locals, const SymbolTable& functions );



// <expr> -> id<array of r>:=<expr> | <or expr>
Type checkExpr( Parser::ParseTree* expr, const SymbolTable& locals, const SymbolTable& functions )
{
	assert( !expr->node.is_leaf );
	assert( expr->node.non_term == NonTerminals::EXPR );

	if( expr->children.size( ) > 2 )	// id<array of r>:=<expr> part
	{
		// we find the type of the id
		SymbolTable::const_iterator temp = locals.find( expr->children[0]->node.token.lexeme );
		if( temp == locals.end( ) )
		{
			std::string msg = "Variable ";
			msg += expr->children[0]->node.token.lexeme;

			throw Error( msg + " not defined", expr->children[0]->node.token.line_number );
		}

		SymbolEntry id_symb = temp->second;
		Type id_type = id_symb.type;

		// we need to check that the type of id (array dim) matches the number of brackets given
		// and loop over the <arr> nodes. We also check that they are expressions resolving to int
		int bracks = 0;
		Parser::ParseTree* curr_arr = expr->children[1];
		while( curr_arr->children.size( ) > 2 )
		{
			Type subscript_type = checkExpr( curr_arr->children[1], locals, functions );
			if( (subscript_type.array_dimension != 0) || (subscript_type.base != Type::INT) )	// not a single int
				throw Error( "Subscripts must evaluate to an integer", id_symb.line_number );

			bracks++;	// one more level of indirection...
			curr_arr = curr_arr->children[3];
		}

		// Check that we have all of the indirection we need...and no more
		if( id_type.array_dimension != bracks )
			throw Error( "Cannot assign to an array, only primitive types", expr->children[0]->node.token.line_number );


		// Now we must check that the LHS and RHS match!
		Type rhs_type = checkExpr( expr->children[3], locals, functions );

		if( rhs_type.base != id_type.base )
			throw Error( "Type of LHS of := does not match that of RHS", id_symb.line_number );

		// We return the type of the rhs
		return rhs_type;
	}
	else	// <or expr> part
	{
		return checkOr( expr->children[0], locals, functions );
	}
}


// <or expr> -> <and expr><or expr'>
// <or expr'> -> |<and expr><or expr'> | e
Type checkOr( Parser::ParseTree* or_n, const SymbolTable& locals, const SymbolTable& functions )
{
	assert( !or_n->node.is_leaf );
	assert( or_n->node.non_term == NonTerminals::OR_EXPR );

	// get the type of <and> part
	Type type_of_and = checkAnd( or_n->children[0], locals, functions );

	// get pointer to the first <or'>
	Parser::ParseTree* curr_orP = or_n->children[1];
	bool had_a_or = false;

	while( curr_orP->children.size( ) > 1 )
	{
		had_a_or = true;

		// We have an actual or operation - <and> must have type bool!
		if( type_of_and.array_dimension != 0 )
			throw Error( "Or operator can only be used with bools", curr_orP->children[0]->node.token.line_number );
		if( type_of_and.base != Type::BOOL )
			throw Error( "Or operator can only be used with bools", curr_orP->children[0]->node.token.line_number );

		// get type of next <and>
		Type next_and = checkAnd( curr_orP->children[1], locals, functions );

		// it must have type bool as well!
		if( next_and.array_dimension != 0 )
			throw Error( "Or operator can only be used with bools", curr_orP->children[0]->node.token.line_number );
		if( next_and.base != Type::BOOL )
			throw Error( "Or operator can only be used with bools", curr_orP->children[0]->node.token.line_number );

		// move on to the next <or'>
		curr_orP = curr_orP->children[2];
	}

	// if we had an or, then type_of_and will be bool
	// if not, we want to return type_of_and :)
	return type_of_and;
}





// <and expr> -> <eq expr><and expr'>
// <and expr'> -> &<eq expr><and expr'> | e
Type checkAnd( Parser::ParseTree* and_n, const SymbolTable& locals, const SymbolTable& functions )
{
	assert( !and_n->node.is_leaf );
	assert( and_n->node.non_term == NonTerminals::AND_EXPR );

	// get the type of the <eq> part
	Type eq_type = checkEq( and_n->children[0], locals, functions );

	// get pointer to the first <and'>
	Parser::ParseTree* curr_andP = and_n->children[1];
	bool had_a_and = false;

	while( curr_andP->children.size( ) > 1 )
	{
		had_a_and = true;

		// We have an and operation, original <eq> must have type bool!
		if( eq_type.array_dimension != 0 )
			throw Error( "And operator can only be used with bools", curr_andP->children[0]->node.token.line_number );
		if( eq_type.base != Type::BOOL )
			throw Error( "And operator can only be used with bools", curr_andP->children[0]->node.token.line_number );

		// get type of next <eq>
		Type next_eq = checkEq( curr_andP->children[1], locals, functions );

		// it must have type bool as well!
		if( next_eq.array_dimension != 0 )
			throw Error( "And operator can only be used with bools", curr_andP->children[0]->node.token.line_number );
		if( next_eq.base != Type::BOOL )
			throw Error( "And operator can only be used with bools", curr_andP->children[0]->node.token.line_number );

		// move on to the next <and'>
		curr_andP = curr_andP->children[2];
	}

	// if we had an and, then eq_type will be bool
	// if not, we want to return eq_type :)
	return eq_type;
}





// <eq expr> -> <rel expr><eq expr'>
// <eq expr'> -> ==<rel expr><eq expr'> | /=<rel expr><eq expr'> | e
Type checkEq( Parser::ParseTree* eq, const SymbolTable& locals, const SymbolTable& functions )
{
	assert( !eq->node.is_leaf );
	assert( eq->node.non_term == NonTerminals::EQ_EXPR );

	// get the type of the <rel> part
	Type rel_type = checkRel( eq->children[0], locals, functions );

	// get pointer to the first <eq'>
	Parser::ParseTree* curr_eqP = eq->children[1];
	bool had_a_eq = false;

	while( curr_eqP->children.size( ) > 1 )
	{
		had_a_eq = true;

		// original rel_type must not be an array!
		if( rel_type.array_dimension != 0 )
			throw Error( "Operand to equality operator cannot be an array type", curr_eqP->children[0]->node.token.line_number );

		// get the next <rel> type
		Type next_rel = checkRel( curr_eqP->children[1], locals, functions );

		// can't be an array
		if( next_rel.array_dimension != 0 )
			throw Error( "Operand to equality operator cannot be an array type", curr_eqP->children[0]->node.token.line_number );

		// Also, the two must have the same type!
		if( rel_type.base != next_rel.base )
			throw Error( "Operands to equality operator must have the same type", curr_eqP->children[0]->node.token.line_number );
		
		// move on to the next <and'>
		curr_eqP = curr_eqP->children[2];
	}

	// if we had an equality operation, the result is bool
	if( had_a_eq )
	{
		Type bool_type;
		bool_type.base = Type::BOOL;
		bool_type.function = false;
		bool_type.array_dimension = 0;

		return bool_type;
	}
	else	// it is the type of the <rel>
	{
		return rel_type;
	}
}






// <rel expr> -> <add expr><rel expr'>
// <rel expr'> -> <<add expr><rel expr'> | ><add expr><rel expr'> | <=<add expr><rel expr'> | >=<add expr><rel expr'> | e
Type checkRel( Parser::ParseTree* rel, const SymbolTable& locals, const SymbolTable& functions )
{
	assert( !rel->node.is_leaf );
	assert( rel->node.non_term == NonTerminals::REL_EXPR );

	// get the type of <add> part (look at teh tree!)
	Type type_of_add = checkAdd( rel->children[0], locals, functions );

	// get pointer to the first <rel'>
	Parser::ParseTree* curr_relP = rel->children[1];
	
	// we need to know if there even was a relational
	bool had_a_rel = false;
	
	while( curr_relP->children.size( ) > 1 )	// While we have a relational operation 
	{
		had_a_rel = true;	// we had >= 1 relational

		// get the type of the next <add>
		Type type_of_next_add = checkAdd( curr_relP->children[1], locals, functions );

		// check for compatibility!
		// relational operators:  <  >  <=  >=   can only be used on int-int real-real and char-char

		if( type_of_add.base == Type::BOOL )	// original must not be bool
			throw Error( "Type Error: LHS of relational operator cannot be bool", curr_relP->children[0]->node.token.line_number );
		if( type_of_add.array_dimension != 0 )	// no whole arrays!
			throw Error( "Type Error: LHS of relational operator cannot be an array", curr_relP->children[0]->node.token.line_number );

		if( type_of_next_add.array_dimension != 0 )	// no whole arrays!
			throw Error( "Type Error: RHS of relational operator cannot be an array", curr_relP->children[0]->node.token.line_number );

		// Now we check for equality!
		if( type_of_add.base != type_of_next_add.base )
			throw Error( "Type Mismatch: Operands to relational operator must have the same types", curr_relP->children[0]->node.token.line_number );


		// move to the next <rel'>
		curr_relP = curr_relP->children[2];
	}

	// Now we must return the type
	// if we had a relational at all, the type is bool
	// otherwise, it is the type of the first add
	if( had_a_rel )
	{
		Type bool_type;
		bool_type.base = Type::BOOL;
		bool_type.function = false;
		bool_type.array_dimension = 0;

		return bool_type;
	}
	else
	{
		return type_of_add;
	}
}




// <add expr> -> <mult expr><add expr'>
// <add expr'> -> +<mult expr><add expr'> | -<mult expr><add expr'> | e
Type checkAdd( Parser::ParseTree* add, const SymbolTable& locals, const SymbolTable& functions )
{
	assert( !add->node.is_leaf );
	assert( add->node.non_term == NonTerminals::ADD_EXPR );

	// Get the type of the <mult>
	Type mult_type = checkMult( add->children[0], locals, functions );
	
	// Get a pointer to the first <add'>
	Parser::ParseTree* curr_addP = add->children[1];
	while( curr_addP->children.size( ) > 1 )
	{
		// we have another add

		// Get the type of the next <mult>
		Type next_mult = checkMult( curr_addP->children[1], locals, functions );

		// Neither can be an array!
		if( (mult_type.array_dimension + next_mult.array_dimension) > 0 )
			throw Error( "Arrays cannot be used as operands to arithmetic operators", curr_addP->children[0]->node.token.line_number );

		// The operands must be int-int real-real or char-char   (real + int is not directly allowed :P)
		if( mult_type.base != next_mult.base )
			throw Error( "Only like types can be added. Use the casting functions to convert one of the types", curr_addP->children[0]->node.token.line_number );
		
		if( mult_type.base == Type::BOOL )
			throw Error( "bool is not a legal type for arithmetic operators", curr_addP->children[0]->node.token.line_number );

		// move on
		curr_addP = curr_addP->children[2];
	}

	// if we had an add, then the type has to be the same as the <mult>
	// if not, we still want the type of the <mult>

	return mult_type;
}


// <mult expr> -> <pow expr><mult expr'>
// <mult expr'> -> *<pow expr><mult expr'> | /<pow expr><mult expr'> | %<pow expr><mult expr'> | e
Type checkMult( Parser::ParseTree* mult, const SymbolTable& locals, const SymbolTable& functions )
{
	assert( !mult->node.is_leaf );
	assert( mult->node.non_term == NonTerminals::MULT_EXPR );

	// Get the type of the <pow>
	Type pow_type = checkPow( mult->children[0], locals, functions );

	// Get a pointer to the first <mult'>
	Parser::ParseTree* curr_multP = mult->children[1];

	while( curr_multP->children.size( ) > 1 )
	{
		// another multiply
		Type next_pow = checkPow( curr_multP->children[1], locals, functions );


		// perform typing checks!!
		// * and / work for int-int and real-real
		// % only works for int-int

		// we can't have arrays
		if( (next_pow.array_dimension + pow_type.array_dimension) > 0 )
			throw Error( "Array types cannot be used in arithmetic", curr_multP->children[0]->node.token.line_number );

		// orig must be int or real
		if( (pow_type.base != Type::REAL) && (pow_type.base != Type::INT) )
			throw Error( "Only int or real can be used in multiplicative expressions", curr_multP->children[0]->node.token.line_number );

		// this must be int or real
		if( (next_pow.base != Type::REAL) && (next_pow.base != Type::INT) )
			throw Error( "Only int or real can be used in multiplicative expressions", curr_multP->children[0]->node.token.line_number );

		// The two must be equal
		if( next_pow.base != pow_type.base )
			throw Error( "Operands to arithmetic operators must have the same type. Use cast functions to convert", curr_multP->children[0]->node.token.line_number );

		// If we have %, they must be integral!
		if( curr_multP->children[0]->node.token.token_type == TokenTypes::PERCENT )
		{
			if( pow_type.base != Type::INT )
				throw Error( "Modulus can only be performed on integers", curr_multP->children[0]->node.token.line_number );
		}

		curr_multP = curr_multP->children[2];
	}

	// we want the type of the original <pow>
	return pow_type;
}


// <pow expr> -> <unary expr>^<pow expr> | <unary expr>
Type checkPow( Parser::ParseTree* pow, const SymbolTable& locals, const SymbolTable& functions )
{
	assert( !pow->node.is_leaf );
	assert( pow->node.non_term == NonTerminals::POW_EXPR );

	if( pow->children.size( ) == 1 )
	{
		// expands to just unary
		return checkUnary( pow->children[0], locals, functions );
	}
	else
	{
		Type unary_type = checkUnary( pow->children[0], locals, functions );
		Type pow_type = checkPow( pow->children[2], locals, functions );

		// exponentiation only works for int-int and real-real
		if( (unary_type.array_dimension + pow_type.array_dimension) > 0 )
			throw Error( "Arrays can not be used as operands to operator ^", pow->children[1]->node.token.line_number );

		if( (unary_type.base == Type::BOOL) || (unary_type.base == Type::CHAR) )
			throw Error( "Only types int and real can be used for exponentiation", pow->children[1]->node.token.line_number );

		if( unary_type.base != pow_type.base )
			throw Error( "Types of operands to operator ^ must match. Use casting functions to convert.", pow->children[1]->node.token.line_number );

		return unary_type;
	}
}

// <unary expr> -> !<unary expr> | -<unary expr> | +<unary expr> | <term expr>
Type checkUnary( Parser::ParseTree* unary, const SymbolTable& locals, const SymbolTable& functions )
{
	assert( !unary->node.is_leaf );
	assert( unary->node.non_term == NonTerminals::UNARY_EXPR );

	if( unary->children.size( ) == 1 )
	{
		// expands to just <term>
		return checkTerm( unary->children[0], locals, functions );
	}
	else
	{
		Type unary_type = checkUnary( unary->children[1], locals, functions );

		// no arrays...
		if( unary_type.array_dimension != 0 )
			throw Error( "Array type not allowed after unary operator", unary->children[0]->node.token.line_number );

		// if it's + or - type should be int or real
		if( unary->children[0]->node.token.token_type != TokenTypes::NOT )
		{
			if( (unary_type.base != Type::REAL) && (unary_type.base != Type::INT) )
				throw Error( "Only int or real can be used for unary + or -.", unary->children[0]->node.token.line_number );
		}
		else	// if it's ! the type should be bool
		{
			if( unary_type.base != Type::BOOL )
				throw Error( "Only type bool can be used with unary !", unary->children[0]->node.token.line_number );
		}

		return unary_type;
	}
}


// <term expr> -> (<expr>) | id<array of r> | literal | <func call>
Type checkTerm( Parser::ParseTree* term, const SymbolTable& locals, const SymbolTable& functions )
{
	assert( !term->node.is_leaf );
	assert( term->node.non_term == NonTerminals::TERM_EXPR );

	if( (term->children[0]->node.is_leaf) && (term->children[0]->node.token.token_type == TokenTypes::LEFT_PARENS) )
	{
		// (<expr>)
		return checkExpr( term->children[1], locals, functions );
	}
	else if( (term->children.size( ) >= 2) && (!term->children[1]->node.is_leaf) && (term->children[1]->node.non_term == NonTerminals::ARRAY_OF_R) )
	{
		// id<array of r>

		// get pointer to first <arr>
		Parser::ParseTree* curr_arr = term->children[1];
		int indexes = 0;

		while( curr_arr->children.size( ) > 2 )	// we have indexing!
		{
			indexes++;

			Type type_of_ind = checkExpr( curr_arr->children[1], locals, functions );

			// check type of index
			if( (type_of_ind.array_dimension != 0) || (type_of_ind.base != Type::INT) )
				throw Error( "Types of array subscripts must have integer type", curr_arr->children[0]->node.token.line_number );

			curr_arr = curr_arr->children[3];
		}

		// find the type of the id
		SymbolTable::const_iterator temp = locals.find( term->children[0]->node.token.lexeme );
		if( temp == locals.end( ) )
		{
			std::string msg = "Variable ";
			msg += term->children[0]->node.token.lexeme;
			
			throw Error( msg + " not defined", term->children[0]->node.token.line_number );
		}

		Type id_type = temp->second.type;

		// the return type is the same except the levels of indirection
		// it will be that of the id minus the number of brackets we used
		Type id_arr_type;
		id_arr_type.function = false;
		id_arr_type.base = id_type.base;
		id_arr_type.array_dimension = id_type.array_dimension - indexes;

		if( (id_type.array_dimension - indexes) < 0 )
			throw Error( "Too many levels of indirection", term->children[0]->node.token.line_number );

		return id_arr_type;
	}
	else if( term->children[0]->node.is_leaf )
	{
		// literal
		Type lit_type;
		lit_type.array_dimension = 0;
		lit_type.function = false;

		switch( term->children[0]->node.token.token_type )
		{
		case TokenTypes::INT_LITERAL:
			lit_type.base = Type::INT;
			break;

		case TokenTypes::REAL_LITERAL:
			lit_type.base = Type::REAL;
			break;

		case TokenTypes::CHAR_LITERAL:
			lit_type.base = Type::CHAR;
			break;

		case TokenTypes::BOOL_LITERAL:
			lit_type.base = Type::BOOL;
			break;

		default:
			throw Error( "Internal Error" );	// should really not happen
		}

		return lit_type;
	}
	else
	{
		// <func call>
		return checkFuncCall( term->children[0], locals, functions );
	}
}


bool typesEqual( const Type& a, const Type& b )
{
	return (a.array_dimension == b.array_dimension) && (a.base == b.base);
}


// <params> -> <expr><next param> | e
void checkParams( Parser::ParseTree* pars, const SymbolTable& locals, const SymbolEntry& function, const SymbolTable& functions, int line )
{
	assert( !pars->node.is_leaf );
	assert( pars->node.non_term == NonTerminals::PARAMS );

	// if no params, return
	if( pars->children.size( ) <= 1 )
		return;

	// check the first parameter
	if( !typesEqual( function.type.parameters[0], checkExpr( pars->children[0], locals, functions ) ) )
		throw Error( "Formal parameter type does not match given parameter type", line );

	// Get a pointer to <next>
	Parser::ParseTree* curr_next = pars->children[1];
	int par = 1;
	while( curr_next->children.size( ) > 2 )
	{
		if( !typesEqual( function.type.parameters[par], checkExpr( curr_next->children[1], locals, functions ) ) )
			throw Error( "Formal parameter type does not match given parameter type", curr_next->children[0]->node.token.line_number );

		curr_next = curr_next->children[2];
		par++;
	}
}



// <func call> -> id(<params>)
Type checkFuncCall( Parser::ParseTree* fcall, const SymbolTable& locals, const SymbolTable& functions )
{
	assert( !fcall->node.is_leaf );
	assert( fcall->node.non_term == NonTerminals::FUNC_CALL );

	// Find this function
	SymbolTable::const_iterator temp = functions.find( fcall->children[0]->node.token.lexeme );
	if( temp == functions.end( ) )
	{
		std::string msg = "Function ";
		msg += fcall->children[0]->node.token.lexeme;
		
		throw Error( msg + " not defined", fcall->children[0]->node.token.line_number );
	}

	SymbolEntry function = temp->second;

	// Check that the parameters work
	checkParams( fcall->children[2], locals, function, functions, fcall->children[0]->node.token.line_number );

	// Type of the function call is the type of the function (minus parens)
	Type func_type = function.type;
	func_type.function = false;
	func_type.parameters.clear( );

	return func_type;
}




// Type Checking
void checkTypes( Parser::ParseTree* tree, const SymbolTable& locals, const SymbolTable& functions, const std::string& func_name  )
{
	// We don't need to check every <expr> because many will be checked inside of the recursion
	// we only need to check ones that derive right off <stmt>

	// we need to handle return values as a special case, since there type correctness does not dpend on
	// related nodes of the tree (like everything else), but the type of the function. We do that here
	static Type this_func_type;

	if( func_name != "" )
	{
		// get the type of the current function
		SymbolTable::const_iterator temp = functions.find( func_name );
		if( temp == functions.end( ) )
			throw Error( "Internal Compiler Error" );	// shole *never* happen
	
		this_func_type = temp->second.type;
	}

	if( !tree->node.is_leaf && (tree->node.non_term == NonTerminals::STMT) )
	{
		// <stmt> -> if(<expr>)<stmt>else<stmt> | while(<expr>)<stmt> | return <expr>; | <expr>;

		if( tree->children[0]->node.is_leaf )	// first child is a leaf
		{
			switch( tree->children[0]->node.token.token_type )
			{
			case TokenTypes::KW_IF:
				checkExpr( tree->children[2], locals, functions );
				break;

			case TokenTypes::KW_WHILE:
				checkExpr( tree->children[2], locals, functions );
				break;

			case TokenTypes::KW_RETURN:
				Type ret_type = checkExpr( tree->children[1], locals, functions );
				if( !typesEqual(ret_type, this_func_type) )
					throw Error( "Type of return vale does not match type of function", tree->children[0]->node.token.line_number );
				break;
			}
		}
		else
		{
			if( tree->children[0]->node.non_term == NonTerminals::EXPR )
			{
				checkExpr( tree->children[0], locals, functions );
			}
		}
	}

	// recurse
	for( unsigned int i=0; i<tree->children.size( ); i++ )
		checkTypes( tree->children[i], locals, functions, "" );
}









