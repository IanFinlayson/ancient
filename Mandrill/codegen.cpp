// codegen.cpp

#include <algorithm>
#include <sstream>
#include <cassert>
#include <cstdio>
#include "codegen.hpp"
#include "errors.hpp"

// These are defined in symbol.cpp
// including the file directly caused circular dependencies this is a slight hack
Type checkOr( Parser::ParseTree* o, const SymbolTable& locals, const SymbolTable& functions );
Type checkAnd( Parser::ParseTree* an, const SymbolTable& locals, const SymbolTable& functions );
Type checkEq( Parser::ParseTree* eq, const SymbolTable& locals, const SymbolTable& functions );
Type checkRel( Parser::ParseTree* rel, const SymbolTable& locals, const SymbolTable& functions );
Type checkAdd( Parser::ParseTree* add, const SymbolTable& locals, const SymbolTable& functions );
Type checkMult( Parser::ParseTree* add, const SymbolTable& locals, const SymbolTable& functions );
Type checkPow( Parser::ParseTree* pow, const SymbolTable& locals, const SymbolTable& functions );
Type checkUnary( Parser::ParseTree* unary, const SymbolTable& locals, const SymbolTable& functions );
Type checkTerm( Parser::ParseTree* term, const SymbolTable& locals, const SymbolTable& functions );
Type checkFuncCall( Parser::ParseTree* fcall, const SymbolTable& locals, const SymbolTable& functions );
Address genExpr( std::vector<Instruct>& code, Parser::ParseTree* expr, SymbolTable& locals, SymbolTable& functions );

void Address::print( std::ostream& os )
{
	assert( isValid( ) );

	if( literal )
	{
		os << lit;
	}
	else
	{
		os << address->second.lexeme;
	}
}



Instruct::Instruct( InterInstrs::InterInstr op, Address arg1, Address arg2, Address result,
				   const std::string& label, int arg_count )
{
	this->op = op;
	this->arg1 = arg1;
	this->arg2 = arg2;
	this->result = result;
	this->label = label;
	this->arg_count = arg_count;

	// Do correctness testing...
	using namespace InterInstrs;
	switch( this->op )
	{
		case MOVE:
		case NOT:
			assert( result.isValid( ) );
			assert( arg1.isValid( ) );
			break;

		case ADD:
		case ADD_S:
		case MULT:
		case MULT_S:
		case SUB:
		case SUB_S:
		case DIV:
		case DIV_S:
		case REM:
		case AND:
		case OR:
		case POW:
		case POW_S:
		case LT:
		case LTE:
		case GT:
		case GTE:
		case EQ:
		case NE:
		case LT_S:
		case LTE_S:
		case GT_S:
		case GTE_S:
		case EQ_S:
		case NE_S:
			assert( result.isValid( ) );
			assert( arg1.isValid( ) );
			assert( arg2.isValid( ) );
			break;

		case FUNCTION:
		case LABEL:
		case GOTO:
			assert( label != "" );
			break;

		case IF:
			assert( arg1.isValid( ) );
			assert( label != "" );
			break;

		case PARAM:
		case LOCAL:
		case ARG:
			assert( arg1.isValid( ) );
			break;

		case CALL:
			assert( label != "" );
			assert( arg_count >= 0 );
			break;

		
		case RETURN:
			assert( arg1.isValid( ) );
			break;

		case INDEXL:
		case INDEXR:
			assert( result.isValid( ) );
			assert( arg1.isValid( ) );
			assert( arg2.isValid( ) );
			break;

		case GETRET:
			assert( result.isValid( ) );
			break;

		default:
			assert( false );
			break;
	}
}

void Instruct::print( std::ostream& os )
{
	using namespace InterInstrs;
	switch( op )
	{
	case MOVE: os << "MOVE "; result.print( os ); os << " "; arg1.print( os ); os << "\n"; break;
	case NOT: os << "NOT "; result.print( os ); os << " "; arg1.print( os ); os << "\n"; break;
	
	case ADD: os << "ADD "; result.print( os ); os  << " "; arg1.print( os ); os << " "; arg2.print( os ); os << "\n"; break;
	case ADD_S: os << "ADD_S "; result.print( os ); os  << " "; arg1.print( os ); os << " "; arg2.print( os ); os << "\n"; break;
	case MULT: os << "MULT "; result.print( os ); os  << " "; arg1.print( os ); os << " "; arg2.print( os ); os << "\n"; break;
	case MULT_S: os << "MULT_S "; result.print( os ); os  << " "; arg1.print( os ); os << " "; arg2.print( os ); os << "\n"; break;
	case SUB: os << "SUB "; result.print( os ); os  << " "; arg1.print( os ); os << " "; arg2.print( os ); os << "\n"; break;
	case SUB_S: os << "SUB_S "; result.print( os ); os  << " "; arg1.print( os ); os << " "; arg2.print( os ); os << "\n"; break;
	case DIV: os << "DIV "; result.print( os ); os  << " "; arg1.print( os ); os << " "; arg2.print( os ); os << "\n"; break;
	case DIV_S: os << "DIV_S "; result.print( os ); os  << " "; arg1.print( os ); os << " "; arg2.print( os ); os << "\n"; break;
	case REM: os << "REM "; result.print( os ); os  << " "; arg1.print( os ); os << " "; arg2.print( os ); os << "\n"; break;
	case AND: os << "AND "; result.print( os ); os  << " "; arg1.print( os ); os << " "; arg2.print( os ); os << "\n"; break;
	case OR: os << "OR "; result.print( os ); os  << " "; arg1.print( os ); os << " "; arg2.print( os ); os << "\n"; break;
	case POW: os << "POW "; result.print( os ); os  << " "; arg1.print( os ); os << " "; arg2.print( os ); os << "\n"; break;
	case POW_S: os << "POW_S "; result.print( os ); os  << " "; arg1.print( os ); os << " "; arg2.print( os ); os << "\n"; break;
	case LT: os << "LT "; result.print( os ); os  << " "; arg1.print( os ); os << " "; arg2.print( os ); os << "\n"; break;
	case LTE: os << "LTE "; result.print( os ); os  << " "; arg1.print( os ); os << " "; arg2.print( os ); os << "\n"; break;
	case GT: os << "GT "; result.print( os ); os  << " "; arg1.print( os ); os << " "; arg2.print( os ); os << "\n"; break;
	case GTE: os << "GTE "; result.print( os ); os  << " "; arg1.print( os ); os << " "; arg2.print( os ); os << "\n"; break;
	case EQ: os << "EQ "; result.print( os ); os  << " "; arg1.print( os ); os << " "; arg2.print( os ); os << "\n"; break;
	case NE: os << "NE "; result.print( os ); os  << " "; arg1.print( os ); os << " "; arg2.print( os ); os << "\n"; break;
	case LT_S: os << "LT_S "; result.print( os ); os  << " "; arg1.print( os ); os << " "; arg2.print( os ); os << "\n"; break;
	case LTE_S: os << "LTE_S "; result.print( os ); os  << " "; arg1.print( os ); os << " "; arg2.print( os ); os << "\n"; break;
	case GT_S: os << "GT_S "; result.print( os ); os  << " "; arg1.print( os ); os << " "; arg2.print( os ); os << "\n"; break;
	case GTE_S: os << "GTE_S "; result.print( os ); os  << " "; arg1.print( os ); os << " "; arg2.print( os ); os << "\n"; break;
	case EQ_S: os << "EQ_S "; result.print( os ); os  << " "; arg1.print( os ); os << " "; arg2.print( os ); os << "\n"; break;
	case NE_S: os << "NE_S "; result.print( os ); os  << " "; arg1.print( os ); os << " "; arg2.print( os ); os << "\n"; break;
	
	case LABEL: os << "LABEL " << label << "\n"; break;
	case GOTO: os << "GOTO " << label << "\n"; break;

	case IF: os << "IF "; arg1.print( os ); os << " " << label << "\n"; break;

	case FUNCTION: os << "\n\nFUNCTION " << label << "\n"; break;
	case PARAM: os << "PARAM "; arg1.print( os ); os << "\n"; break;
	case ARG: os << "ARG "; arg1.print( os ); os << "\n"; break;
	case LOCAL: os << "LOCAL "; arg1.print( os ); os << "\n"; break;

	case CALL: os << "CALL " << label << " " << arg_count << "\n"; break;
	case RETURN: os << "RETURN "; arg1.print( os ); os << "\n"; break;

	case INDEXL: os << "INDEXL "; result.print( os ); os << " "; arg1.print( os ); os << " "; arg2.print( os ); os << "\n"; break;
	case INDEXR: os << "INDEXR "; result.print( os ); os << " "; arg1.print( os ); os << " "; arg2.print( os ); os << "\n"; break;
	
	case GETRET: os << "GETRET "; result.print( os ); os << "\n"; break;

	default: assert( false ); break;
	}
}

// return a unique string for ids and labels
std::string getUniqueName( bool id )	// if not id, then label
{
	static int id_num = 0;
	static int la_num = 0;

	std::stringstream s;
	if( id )
	{
		s << "t" << id_num;
		id_num++;
	}
	else
	{
		s << "label" << la_num;
		la_num++;
	}

	return s.str( );
}




// Generate a temporary address of type type
// and insert it into the symbol table
Address genTemp( SymbolTable& locals, Type type )
{
	std::string name = getUniqueName( true );
	SymbolEntry temp;
	temp.lexeme = name;
	temp.line_number = -1;
	temp.type = type;

	std::pair<std::string, SymbolEntry> in( name, temp );

	std::pair<SymbolTable::iterator, bool> results = locals.insert( in );

	if( !results.second ) throw Error( "You cannot begin an identifier with mandrillTemp. They are reserved for the implementation" );

	return Address( results.first );
}



// Generates code for function calls
Address genFuncCall( std::vector<Instruct>& code, Parser::ParseTree* node, SymbolTable& locals, SymbolTable& functions )
{
	// First we need to generate the PARAM code
	int num_params = 0;
	Parser::ParseTree* params = node->children[2];
	
	if( params->children.size( ) > 1 )
	{
		// we have params
		num_params++;
		
		// do the first one
		Address par = genExpr( code, params->children[0], locals, functions );

		// Write th PARAM
		code.push_back( Instruct( InterInstrs::PARAM, par ) );

		Parser::ParseTree* curr_next = params->children[1];
		while( curr_next->children.size( ) > 1 )
		{
			// do the next one
			num_params++;

			par = genExpr( code, curr_next->children[1], locals, functions );

			code.push_back( Instruct( InterInstrs::PARAM, par ) );

			curr_next = curr_next->children[2];
		}
	}

	std::string func_name = node->children[0]->node.token.lexeme;

	// Now we do the function call
	code.push_back( Instruct( InterInstrs::CALL, Address(), Address(), Address(), func_name, num_params ) );

	// get the function type
	Type func_type = functions.find( func_name )->second.type;
	// The func_type's return value does not include params...
	func_type.function = false;
	func_type.parameters.clear( );

	// the return value
	Address val = genTemp( locals, func_type );
	code.push_back( Instruct( InterInstrs::GETRET, Address(), Address(), val ) );

	return val;
}




// generates code to calculate indices and returns an Address
Address genIdArr( std::vector<Instruct>& code, Parser::ParseTree* node, SymbolTable& locals, SymbolTable& functions )
{
	// the node can be various types, but expands to id<array of r> as the first two children!!!
	assert( node );

	std::string id_name = node->children[0]->node.token.lexeme;
	SymbolTable::iterator symbol = locals.find( id_name );

	Address address( symbol );

	// Now we need to find the index if it's an array...
	Parser::ParseTree* curr_arr = node->children[1];
	int i = 0;
	Type int_t;		int_t.array_dimension=0; int_t.function=false; int_t.base=Type::INT;
	
	Address* offset = NULL;
	if( curr_arr->children.size( ) > 1 )
	{
		offset = new Address( genTemp( locals, int_t) );	// must be freed at shutdown...
		address.setOffset( offset );

		Address zero( int_t.base, "0" );
		code.push_back( Instruct( InterInstrs::MOVE, zero, Address(), *offset ) );
	}

	Address temp = genTemp( locals, int_t );
	while( curr_arr->children.size( ) > 1 )
	{
		Address index = genExpr( code, curr_arr->children[1], locals, functions );

		// Generate the code to calculate the offset
		// and make sure the result gets stored in address!
		if( curr_arr->children[3]->children.size( ) <= 1 )
		{
			// This is the last one...Add index to offset!
			code.push_back( Instruct( InterInstrs::ADD, *offset, index, *offset ) );
			curr_arr = curr_arr->children[3];
			i++;
		}
		else
		{
			char i_need_boost[32]; sprintf( i_need_boost, "%i", symbol->second.type.array_sizes[i] );
			std::string dimension_str( i_need_boost );

			Address dim( int_t.base, dimension_str );

			// Now we want to do a multiply and an add
			code.push_back( Instruct( InterInstrs::MULT, index, dim, temp ) );
			code.push_back( Instruct( InterInstrs::ADD, *offset, temp, *offset ) );

			curr_arr = curr_arr->children[3];
			i++;
		}
	}

	return address;
}



// generates code for term expressions
Address genTermExpr( std::vector<Instruct>& code, Parser::ParseTree* term, SymbolTable& locals, SymbolTable& functions )
{
	assert( term );
	assert( !term->node.is_leaf );
	assert( term->node.non_term == NonTerminals::TERM_EXPR );

	// <term expr> -> (<expr>) | id<array of r> | literal | <func call>

	if( term->children[0]->node.is_leaf )
	{
		switch( term->children[0]->node.token.token_type )
		{
		case TokenTypes::LEFT_PARENS:
			return genExpr( code, term->children[1], locals, functions );

		case TokenTypes::IDENTIFIER:
			return genIdArr( code, term, locals, functions );

		case TokenTypes::INT_LITERAL:
			return Address( Type::INT, term->children[0]->node.token.lexeme );

		case TokenTypes::REAL_LITERAL:
			return Address( Type::REAL, term->children[0]->node.token.lexeme );

		case TokenTypes::BOOL_LITERAL:
			{
				std::string bool_lit = (term->children[0]->node.token.lexeme == "true") ? "1" : "0";
				return Address( Type::BOOL, bool_lit );
			}

		case TokenTypes::CHAR_LITERAL:
			{
				int code = (int) term->children[0]->node.token.lexeme[1];
				char temp[32];
				sprintf( temp, "%i", code );
				
				return Address( Type::CHAR, std::string(temp) );
			}

		default:
			throw Error( "" );
		}
	}
	else
	{
		return genFuncCall( code, term->children[0], locals, functions );
	}
}




// generates code for unary expressions
Address genUnaryExpr( std::vector<Instruct>& code, Parser::ParseTree* unary, SymbolTable& locals, SymbolTable& functions )
{
	assert( unary );
	assert( !unary->node.is_leaf );
	assert( unary->node.non_term == NonTerminals::UNARY_EXPR );

	if( unary->children.size( ) <= 1 )
	{
		// <unary> -> <term>
		return genTermExpr( code, unary->children[0], locals, functions );
	}
	else
	{
		// <unary> -> {!-+} unary
		
		// switch on unary operator
		switch( unary->children[0]->node.token.token_type )
		{
			// unary not !a
		case TokenTypes::NOT:
			{
				Address un = genUnaryExpr( code, unary->children[1], locals, functions );
				Type un_type = checkUnary( unary->children[1], locals, functions );
				Address result = genTemp( locals, un_type );

				code.push_back( Instruct( InterInstrs::NOT, un, Address(), result ) );
				return result;
				break;
			}


		case TokenTypes::ADD:
			// The unary + does nothing! Why even have it??
			return genUnaryExpr( code, unary->children[1], locals, functions );
			break;


			// Unary minus
		case TokenTypes::SUBTRACT:
			{
				Address un = genUnaryExpr( code, unary->children[1], locals, functions );
				Type un_type = checkUnary( unary->children[1], locals, functions );
				if( un_type.base == Type::REAL )
				{
					Address zip( Type::REAL, "0.0" );
					Address result = genTemp( locals, un_type );
					code.push_back( Instruct( InterInstrs::SUB_S, zip, un, result ) );

					return result;
				}
				else
				{
					Address zip( Type::INT, "0" );
					Address result = genTemp( locals, un_type );
					code.push_back( Instruct( InterInstrs::SUB, zip, un, result ) );

					return result;				
				}
				break;
			}
		default:
			throw Error( "" );
		}
	}
}




// generates code for power expressions
Address genPowExpr( std::vector<Instruct>& code, Parser::ParseTree* pow, SymbolTable& locals, SymbolTable& functions )
{
	assert( pow );
	assert( !pow->node.is_leaf );
	assert( pow->node.non_term == NonTerminals::POW_EXPR );

	if( pow->children.size( ) <= 1 )
	{
		// <pow> -> <unary>
		return genUnaryExpr( code, pow->children[0], locals, functions );
	}
	else
	{
		// <pow> -> <unary> ^ <pow>
		Address lhs = genUnaryExpr( code, pow->children[0], locals, functions );
		Address rhs = genPowExpr( code, pow->children[2], locals, functions );

		Type unary_type = checkUnary( pow->children[0], locals, functions );
		Address result = genTemp( locals, unary_type );

		if( unary_type.base == Type::REAL )
			code.push_back( Instruct( InterInstrs::POW_S, lhs, rhs, result ) );
		else
			code.push_back( Instruct( InterInstrs::POW, lhs, rhs, result ) );

		return result;
	}
}

// generates code for multiplicative expressions
Address genMultExpr( std::vector<Instruct>& code, Parser::ParseTree* mult, SymbolTable& locals, SymbolTable& functions )
{
	assert( mult );
	assert( !mult->node.is_leaf );
	assert( mult->node.non_term == NonTerminals::MULT_EXPR );


	// Get the address of the mult
	Address lhs = genPowExpr( code, mult->children[0], locals, functions );
	
	// Get the type of the <mult>
	Type pow_type = checkPow( mult->children[0], locals, functions );


	Parser::ParseTree* curr_multp = mult->children[1];

	// Generate a temp of the same type
	Address result;
	
	// ONLY GEN TEMPS IF NEEDED
	if( curr_multp->children.size( ) > 1 ) result = genTemp( locals, pow_type );

	while( curr_multp->children.size( ) > 1 )
	{
		Address rhs = genPowExpr( code, curr_multp->children[1], locals, functions );
		InterInstrs::InterInstr op;

		if( curr_multp->children[0]->node.token.token_type == TokenTypes::MULTIPLY )
		{
			if( pow_type.base == Type::REAL )
				op = InterInstrs::MULT_S;
			else
				op = InterInstrs::MULT;
		}
		else if( curr_multp->children[0]->node.token.token_type == TokenTypes::DIVIDE )
		{
			if( pow_type.base == Type::REAL )
				op = InterInstrs::DIV_S;
			else
				op = InterInstrs::DIV;
		}
		else
		{
            // Modulus is always integral
			op = InterInstrs::REM;
		}

		code.push_back( Instruct( op, lhs, rhs, result ) );

		lhs = result;
		result = genTemp( locals, pow_type );
		curr_multp = curr_multp->children[2];
	}

	return lhs;
}


// generates code for addition expressions
Address genAddExpr( std::vector<Instruct>& code, Parser::ParseTree* add, SymbolTable& locals, SymbolTable& functions )
{
	assert( add );
	assert( !add->node.is_leaf );
	assert( add->node.non_term == NonTerminals::ADD_EXPR );

	// Get the address of the mult
	Address lhs = genMultExpr( code, add->children[0], locals, functions );
	
	// Get the type of the <mult>
	Type mult_type = checkMult( add->children[0], locals, functions );

	Parser::ParseTree* curr_addp = add->children[1];

	// Generate a temp of the same type
	Address result;
	
	if( curr_addp->children.size( ) > 1 ) result = genTemp( locals, mult_type );

	while( curr_addp->children.size( ) > 1 )
	{
		Address rhs = genMultExpr( code, curr_addp->children[1], locals, functions );
		InterInstrs::InterInstr op;

		if( curr_addp->children[0]->node.token.token_type == TokenTypes::ADD )
		{
			if( mult_type.base == Type::REAL )
				op = InterInstrs::ADD_S;
			else
				op = InterInstrs::ADD;
		}
		else
		{
			if( mult_type.base == Type::REAL )
				op = InterInstrs::SUB_S;
			else
				op = InterInstrs::SUB;
		}

		code.push_back( Instruct( op, lhs, rhs, result ) );

		lhs = result;
		result = genTemp( locals, mult_type );
		curr_addp = curr_addp->children[2];
	}

	return lhs;
}



// generates code for relational expressions
Address genRelExpr( std::vector<Instruct>& code, Parser::ParseTree* rel, SymbolTable& locals, SymbolTable& functions )
{
	assert( rel );
	assert( !rel->node.is_leaf );
	assert( rel->node.non_term == NonTerminals::REL_EXPR );


	// Get the address of the <add>	
	Address lhs = genAddExpr( code, rel->children[0], locals, functions );

	// Now we need to know the type of the <add>
	Type add_type = checkAdd( rel->children[0], locals, functions );


	Parser::ParseTree* curr_relp = rel->children[1];

	// Result should have type bool
	Address result;
	Type b_t; b_t.base = Type::BOOL; b_t.array_dimension = 0; b_t.function = false;
	if( curr_relp->children.size( ) > 1 ) result = genTemp( locals, b_t );


	while( curr_relp->children.size( ) > 1 )
	{
		Address rhs = genAddExpr( code, curr_relp->children[1], locals, functions );
		InterInstrs::InterInstr op;

		switch( curr_relp->children[0]->node.token.token_type )
		{
		case TokenTypes::LESS_THAN:
			op = InterInstrs::LT;
			break;
			
		case TokenTypes::LESS_THAN_EQ:
			op = InterInstrs::LTE;
			break;

		case TokenTypes::GREATER_THAN:
			op = InterInstrs::GT;
			break;

		case TokenTypes::GREATER_THAN_EQ:
			op = InterInstrs::GTE;
			break;

		default:
			throw Error( "" );
		}

		if( add_type.base == Type::REAL )
		{
			// This is a hack!!!!!!!!!!!
			op = (InterInstrs::InterInstr)((int)op + 1);
		}

		code.push_back( Instruct( op, lhs, rhs, result ) );

		lhs = result;
		result = genTemp( locals, b_t );
		curr_relp = curr_relp->children[2];
	}

	return lhs;
}



// code generation of <eq> expressions
Address genEqExpr( std::vector<Instruct>& code, Parser::ParseTree* eq, SymbolTable& locals, SymbolTable& functions )
{
	assert( eq );
	assert( !eq->node.is_leaf );
	assert( eq->node.non_term == NonTerminals::EQ_EXPR );

	// Get the address of the <rel>	
	Address lhs = genRelExpr( code, eq->children[0], locals, functions );

	// Now we need to know the type of the <rel>
	Type rel_type = checkRel( eq->children[0], locals, functions );

	Parser::ParseTree* curr_eqp = eq->children[1];

	Address result;
	Type b_t; b_t.base = Type::BOOL; b_t.array_dimension = 0; b_t.function = false;
	if( curr_eqp->children.size( ) > 1 ) result = genTemp( locals, b_t );

	while( curr_eqp->children.size( ) > 1 )
	{
		Address rhs = genRelExpr( code, curr_eqp->children[1], locals, functions );
		InterInstrs::InterInstr op;
		if( curr_eqp->children[0]->node.token.token_type == TokenTypes::EQUALS )
		{
			if( rel_type.base != Type::REAL )
				op = InterInstrs::EQ;
			else
				op = InterInstrs::EQ_S;
		}
		else
		{
			if( rel_type.base != Type::REAL )
				op = InterInstrs::NE;
			else
				op = InterInstrs::NE_S;
		}

		code.push_back( Instruct( op, lhs, rhs, result ) );

		lhs = result;
		result = genTemp( locals, b_t );
		curr_eqp = curr_eqp->children[2];
	}

	return lhs;
}




// code generation of <and> expressions
Address genAndExpr( std::vector<Instruct>& code, Parser::ParseTree* and_n, SymbolTable& locals, SymbolTable& functions )
{
	assert( and_n );
	assert( !and_n->node.is_leaf );
	assert( and_n->node.non_term == NonTerminals::AND_EXPR );

	// Create a temporary symbol (boolean)
	Type a;	a.base = Type::BOOL; a.array_dimension = 0; a.function = false;
	
	Address lhs = genEqExpr( code, and_n->children[0], locals, functions );

	Parser::ParseTree* curr_andp = and_n->children[1];

	Address result;
	if( curr_andp->children.size( ) > 1 ) result = genTemp( locals, a );

	while( curr_andp->children.size( ) > 1 )
	{
		Address rhs = genEqExpr( code, curr_andp->children[1], locals, functions );
		code.push_back( Instruct( InterInstrs::AND, lhs, rhs, result ) );

		lhs = result;
		result = genTemp( locals, a );
		curr_andp = curr_andp->children[2];
	}

	return lhs;
}



// code generation of <or> expressions (This is the first I wrote...all above of same form follow suit!)
Address genOrExpr( std::vector<Instruct>& code, Parser::ParseTree* or_n, SymbolTable& locals, SymbolTable& functions )
{
	assert( or_n );
	assert( !or_n->node.is_leaf );
	assert( or_n->node.non_term == NonTerminals::OR_EXPR );

	// Create a temporary symbol!!!
	Type a;	a.base = Type::BOOL; a.array_dimension = 0; a.function = false;
	
	Address lhs = genAndExpr( code, or_n->children[0], locals, functions );

	Parser::ParseTree* curr_orp = or_n->children[1];

	Address result;
	if( curr_orp->children.size( ) > 1 ) result = genTemp( locals, a );

	while( curr_orp->children.size( ) > 1 )
	{
		Address rhs = genAndExpr( code, curr_orp->children[1], locals, functions );
		code.push_back( Instruct( InterInstrs::OR, lhs, rhs, result ) );

		lhs = result;
		result = genTemp( locals, a );
		curr_orp = curr_orp->children[2];
	}

	return lhs;
}


// code generation of expressions. We return the address where the result will be stored
Address genExpr( std::vector<Instruct>& code, Parser::ParseTree* expr, SymbolTable& locals, SymbolTable& functions )
{
	assert( expr );
	assert( !expr->node.is_leaf );
	assert( expr->node.non_term == NonTerminals::EXPR );

	// id<array of r>:=<expr> | <or expr>
	
	if( expr->children.size( ) > 1 )
	{
		// Get address of the <expr>
		Address rhs = genExpr( code, expr->children[3], locals, functions );
		
		// Get the address of the id<arr>
		Address lhs = genIdArr( code, expr, locals, functions );

		// Generate the move instruction!
		code.push_back( Instruct( InterInstrs::MOVE, rhs, Address(), lhs ) );

		// return lhs (result of a := b is a)
		return lhs;
	}
	else
	{
		return genOrExpr( code, expr->children[0], locals, functions );
	}
}


// generate code for a list of statements
void genStmt( std::vector<Instruct>& code, Parser::ParseTree* stmt, SymbolTable& locals, SymbolTable& functions );	// forward...
void genStmtList( std::vector<Instruct>& code, Parser::ParseTree* sl, SymbolTable& locals, SymbolTable& functions )
{
	assert( sl );
	assert( !sl->node.is_leaf );
	assert( sl->node.non_term == NonTerminals::STMT_LIST );

	// <stmt><stmt list> | e

	if( sl->children.size( ) > 1 )
	{
		genStmt( code, sl->children[0], locals, functions );
		genStmtList( code, sl->children[1], locals, functions );
	}
}


// code generation of a statement
void genStmt( std::vector<Instruct>& code, Parser::ParseTree* stmt, SymbolTable& locals, SymbolTable& functions )
{
	assert( stmt );
	assert( !stmt->node.is_leaf );
	assert( stmt->node.non_term == NonTerminals::STMT );

	// We do very different things depending on the type of stmt
	if( stmt->children[0]->node.is_leaf )
	{
		switch( stmt->children[0]->node.token.token_type )
		{
			case TokenTypes::KW_IF:
			{
				// if(<expr>)<stmt>else<stmt>

				// get the thingy we test and 2 labels
				Address test = genExpr( code, stmt->children[2], locals, functions );
				std::string labTHEN = getUniqueName( false );
				std::string labEND = getUniqueName( false );

				// IF test, GOTO then
				code.push_back( Instruct( InterInstrs::IF, test, Address(), Address(), labTHEN ) );

				// generate else code
				genStmt( code, stmt->children[6], locals, functions );
				// skip past then part
				code.push_back( Instruct( InterInstrs::GOTO, Address(), Address(), Address(), labEND ) );

				// write the THEN label
				code.push_back( Instruct( InterInstrs::LABEL, Address(), Address(), Address(), labTHEN ) );
				// generate then code
				genStmt( code, stmt->children[4], locals, functions );

				// Write the labEND
				code.push_back( Instruct( InterInstrs::LABEL, Address(), Address(), Address(), labEND ) );
				break;
			}

			case TokenTypes::KW_WHILE:
			{
				// while(<expr>)<stmt>
				
				// We need 3 labels
				std::string labBeg = getUniqueName( false );
				std::string labCon = getUniqueName( false );
				std::string labEnd = getUniqueName( false );

				// LABEL BEGIN
				code.push_back( Instruct( InterInstrs::LABEL, Address(), Address(), Address(), labBeg ) );

				// Get the test code
				Address test = genExpr( code, stmt->children[2], locals, functions );

				// IF test continue
				code.push_back( Instruct( InterInstrs::IF, test, Address(), Address(), labCon ) );

				// GOTO end
				code.push_back( Instruct( InterInstrs::GOTO, Address(), Address(), Address(), labEnd ) );

				// LABEL continue
				code.push_back( Instruct( InterInstrs::LABEL, Address(), Address(), Address(), labCon ) );

				// do the stmt
				genStmt( code, stmt->children[4], locals, functions );

				// goto begin
				code.push_back( Instruct( InterInstrs::GOTO, Address(), Address(), Address(), labBeg ) );

				// label end
				code.push_back( Instruct( InterInstrs::LABEL, Address(), Address(), Address(), labEnd ) );
				break;
			}

			case TokenTypes::KW_BEGIN:
			{
				// begin<stmt list>end
				// just generate the <stmt list>
				genStmtList( code, stmt->children[1], locals, functions );
				break;
			}

			case TokenTypes::KW_RETURN:
			{
				// return <expr>;
				Address ra = genExpr( code, stmt->children[1], locals, functions );
				code.push_back( Instruct( InterInstrs::RETURN, ra ) );
				break;
			}
		}
	}
	else
	{
		if( stmt->children[0]->node.non_term == NonTerminals::EXPR )
		{
			// generate <expr>
			genExpr( code, stmt->children[0], locals, functions );
		}
		// else it's a <symbol decl> which we can ignore
	}
}


// Code generation of a function
void genFunc( std::vector<Instruct>& code, Parser::ParseTree* func, SymbolTable& locals, SymbolTable& functions )
{
	assert( func );
	assert( !func->node.is_leaf );
	assert( func->node.non_term == NonTerminals::FUNCTION );

	// FUNCTION
	code.push_back( Instruct( InterInstrs::FUNCTION, Address(), Address(), Address(), func->children[0]->children[0]->node.token.lexeme ) );

	// We need to get a list of arguments!
	std::vector<Address> arguments;
	Parser::ParseTree* args = func->children[2];
	if( args->children.size( ) > 1 )
	{
		// We have one argument (at least)
		arguments.push_back( locals.find(args->children[0]->children[0]->node.token.lexeme) );

		Parser::ParseTree* next = args->children[1];
		while( next->children.size( ) > 1 )
		{
			// another!
			arguments.push_back( locals.find(next->children[1]->children[0]->node.token.lexeme) );

			next = next->children[2];
		}
	}

	// Print the args
	for( unsigned int i=0; i<arguments.size( ); i++ )
	{
		code.push_back( Instruct( InterInstrs::ARG, arguments[i] ) );
	}

	// Push back the locals
	for( SymbolTable::iterator i=locals.begin( ); i!=locals.end( ); ++i )
	{
		// Check that it is not an argument
		Address temp(i);
		if( std::count(arguments.begin( ), arguments.end( ), temp) == 0 )
			code.push_back( Instruct( InterInstrs::LOCAL, temp ) );
	}

	genStmt( code, func->children[4], locals, functions );
}
















