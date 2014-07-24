// parse.cpp

#include "parse.hpp"
#include "errors.hpp"

// These functions form the recursive descent parser

// A mandrill function
Parser::ParseTree* Parser::function( )
{
	// <function> -> <symbol decl>(<formal args>)<stmt>

	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::FUNCTION;

	// <symbol decl>
	new_node->children.push_back( symbolDecl( ) );
	
	// (
	if( function_info->first[token_counter].token_type == TokenTypes::LEFT_PARENS )
	{
		// Add the nonterminal leaf!
		ParseTree* child = new ParseTree;
		child->node.is_leaf = true;
		child->node.token = function_info->first[token_counter];

		new_node->children.push_back( child );

		token_counter++;
	}
	else
	{
		throw Error( "Parse Error: Expected a (", function_info->first[token_counter].line_number );
	}

	// <formal args>
	new_node->children.push_back( formalArgs( ) );

	// )
	if( function_info->first[token_counter].token_type == TokenTypes::RIGHT_PARENS )
	{
		// Add the nonterminal leaf!
		ParseTree* child = new ParseTree;
		child->node.is_leaf = true;
		child->node.token = function_info->first[token_counter];

		new_node->children.push_back( child );

		token_counter++;
	}
	else
	{
		throw Error( "Parse Error: Expected a )", function_info->first[token_counter].line_number );
	}

	// <stmt>
	new_node->children.push_back( stmt( ) );

	return new_node;
}




// A symbol declaration
Parser::ParseTree* Parser::symbolDecl( )
{
	// <symbol decl> -> id:<type>
	
	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::SYMBOL_DECL;

	// id
	if( function_info->first[token_counter].token_type == TokenTypes::IDENTIFIER )
	{
		// Add the nonterminal leaf!
		ParseTree* child = new ParseTree;
		child->node.is_leaf = true;
		child->node.token = function_info->first[token_counter];

		new_node->children.push_back( child );
		token_counter++;
	}
	else
	{
		throw Error( "Expeceted an identifier", function_info->first[token_counter].line_number );
	}

	// :
	if( function_info->first[token_counter].token_type == TokenTypes::COLON )
	{
		// Add the nonterminal leaf!
		ParseTree* child = new ParseTree;
		child->node.is_leaf = true;
		child->node.token = function_info->first[token_counter];

		new_node->children.push_back( child );
		token_counter++;
	}
	else
	{
		throw Error( "Colon Needed for Declarations", function_info->first[token_counter].line_number );
	}

	// <type>
	new_node->children.push_back( type( ) );

	return new_node;
}






// Arguments supplied when defining a function
Parser::ParseTree* Parser::formalArgs( )
{
	// <formal args> -> <symbol decl><next formal arg> | e

	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::FORMAL_ARGS;

	// we must decide if we have a symbol or not!
	// symbols begin with ids...so we check that

	if( function_info->first[token_counter].token_type == TokenTypes::IDENTIFIER )
	{
		new_node->children.push_back( symbolDecl( ) );
		new_node->children.push_back( nextFormalArg( ) );
	}
	else	// EPSILON CHOICE!!!
	{
		// we have got epsilon
		ParseTree* child = new ParseTree;
		child->node.is_leaf = true;
		child->node.token.token_type = TokenTypes::EPSILON;

		new_node->children.push_back( child );
	}


	return new_node;
}





// The next formal argument
Parser::ParseTree* Parser::nextFormalArg( )
{
	// <next formal arg> -> ,<symbol decl><next formal arg> | e
		
	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::NEXT_FORMAL_ARG;

	// we decide which path to choose!
	if( function_info->first[token_counter].token_type == TokenTypes::COMMA )
	{
		// Add the nonterminal leaf comma
		ParseTree* child = new ParseTree;
		child->node.is_leaf = true;
		child->node.token = function_info->first[token_counter];

		new_node->children.push_back( child );
		token_counter++;

		new_node->children.push_back( symbolDecl( ) );
		new_node->children.push_back( nextFormalArg( ) );
	}
	else	// EPSILON CHOICE!!
	{
		// we have got epsilon
		ParseTree* child = new ParseTree;
		child->node.is_leaf = true;
		child->node.token.token_type = TokenTypes::EPSILON;

		new_node->children.push_back( child );
	}

	return new_node;
}





// a type
Parser::ParseTree* Parser::type( )
{
	// <type> -> <base type><array of c>
	
	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::TYPE;

	// <base type>
	new_node->children.push_back( baseType( ) );

	// <array of c>
	new_node->children.push_back( arrayOfC( ) );

	return new_node;
}





// a base type (no arrays)
Parser::ParseTree* Parser::baseType( )
{
	// <base type> -> int | real | char | bool
	
	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::BASE_TYPE;

	ParseTree* child = new ParseTree;
	child->node.is_leaf = true;
	
	switch( function_info->first[token_counter].token_type )
	{
	case TokenTypes::KW_INT:
		break;
	case TokenTypes::KW_REAL:
		break;
	case TokenTypes::KW_CHAR:
		break;
	case TokenTypes::KW_BOOL:
		break;

	default:
		throw Error( "Type Expected", function_info->first[token_counter].line_number );
	}

	child->node.token = function_info->first[token_counter];
	token_counter++;

	new_node->children.push_back( child );

	return new_node;
}



// An array of (compile time). That is an array with
// compile-time size (an int literal, since Mandrill has no constants)
Parser::ParseTree* Parser::arrayOfC( )
{
	// <array of c> -> [int_literal]<array of c> | e

	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::ARRAY_OF_C;

	// [
	if( function_info->first[token_counter].token_type == TokenTypes::LEFT_BRACKET )
	{
		ParseTree* lb = new ParseTree;
		lb->node.is_leaf = true;
		lb->node.token = function_info->first[token_counter];
		new_node->children.push_back( lb );
		
		token_counter++;

		// int_literal
		if( function_info->first[token_counter].token_type == TokenTypes::INT_LITERAL )
		{
			ParseTree* il = new ParseTree;
			il->node.is_leaf = true;
			il->node.token = function_info->first[token_counter];
			new_node->children.push_back( il );
			
			token_counter++;

			// ]
			if( function_info->first[token_counter].token_type == TokenTypes::RIGHT_BRACKET )
			{
				ParseTree* rb = new ParseTree;
				rb->node.is_leaf = true;
				rb->node.token = function_info->first[token_counter];
				new_node->children.push_back( rb );
				token_counter++;
			}
			else
			{
				throw Error( "Expected a ]", function_info->first[token_counter].line_number );
			}
		}
		else
		{
			throw Error( "Only integer literals can be used for array sizes", function_info->first[token_counter].line_number );
		}

		// recurisve call to <array of C>
		new_node->children.push_back( arrayOfC( ) );
	}
	else
	{
		// we have got epsilon
		ParseTree* child = new ParseTree;
		child->node.is_leaf = true;
		child->node.token.token_type = TokenTypes::EPSILON;

		new_node->children.push_back( child );
	}

	return new_node;
}



// An array of (runtime) - that is, any expression
// may be used as array subscripts
Parser::ParseTree* Parser::arrayOfR( )
{
	// <array of r> -> [<expr>]<array of r> | e

	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::ARRAY_OF_R;

	// [
	if( function_info->first[token_counter].token_type == TokenTypes::LEFT_BRACKET )
	{
		ParseTree* lb = new ParseTree;
		lb->node.is_leaf = true;
		lb->node.token = function_info->first[token_counter];
		new_node->children.push_back( lb );
		token_counter++;


		// <expr>
		new_node->children.push_back( expr( ) );


		// ]
		if( function_info->first[token_counter].token_type == TokenTypes::RIGHT_BRACKET )
		{
			ParseTree* rb = new ParseTree;
			rb->node.is_leaf = true;
			rb->node.token = function_info->first[token_counter];
			new_node->children.push_back( rb );
			token_counter++;
		}
		else
		{
			throw Error( "Expected a ]", function_info->first[token_counter].line_number );
		}

		// recursive call
		new_node->children.push_back( arrayOfR( ) );
	}
	else
	{
		// we have got epsilon
		ParseTree* child = new ParseTree;
		child->node.is_leaf = true;
		child->node.token.token_type = TokenTypes::EPSILON;

		new_node->children.push_back( child );
	}

	return new_node;
}



// A statement
Parser::ParseTree* Parser::stmt( )
{
	//  <stmt> -> if(<expr>)<stmt>else<stmt>
	//          | while(<expr>)<stmt>
	//          | begin<stmt list>end
	//          | return <expr>;
	//          | <expr>;
	//          | <symbol decl>;
	//          | ;

	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::STMT;

	switch( function_info->first[token_counter].token_type )
	{
	
	// if(<expr>)<stmt>else<stmt>
	case TokenTypes::KW_IF:
		{							// Note: {} used to creat scope only
			// IF statement!!
			ParseTree* iF = new ParseTree;
			iF->node.is_leaf = true;
			iF->node.token = function_info->first[token_counter];
			new_node->children.push_back( iF );
			token_counter++;

			// (
			if( function_info->first[token_counter].token_type != TokenTypes::LEFT_PARENS )
				throw Error( "( Expected!", function_info->first[token_counter].line_number );
			
			ParseTree* lp = new ParseTree;
			lp->node.is_leaf = true;
			lp->node.token = function_info->first[token_counter];
			new_node->children.push_back( lp );
			token_counter++;

			// <expr>
			new_node->children.push_back( expr( ) );

			// )
			if( function_info->first[token_counter].token_type != TokenTypes::RIGHT_PARENS )
				throw Error( ") Expected!", function_info->first[token_counter].line_number );

			ParseTree* rp = new ParseTree;
			rp->node.is_leaf = true;
			rp->node.token = function_info->first[token_counter];
			new_node->children.push_back( rp );
			token_counter++;

			// <stmt>
			new_node->children.push_back( stmt( ) );

			// else
			if( function_info->first[token_counter].token_type != TokenTypes::KW_ELSE )
			{
				throw Error( "else clause is Required!", function_info->first[token_counter].line_number );
			}

			ParseTree* eL = new ParseTree;
			eL->node.is_leaf = true;
			eL->node.token = function_info->first[token_counter];
			new_node->children.push_back( eL );
			token_counter++;

			// <stmt>
			new_node->children.push_back( stmt( ) );
		}
		break;









	// while(<expr>)<stmt>
	case TokenTypes::KW_WHILE:
		{
			// While statement!!
			ParseTree* wh = new ParseTree;
			wh->node.is_leaf = true;
			wh->node.token = function_info->first[token_counter];
			new_node->children.push_back( wh );
			token_counter++;

			// (
			if( function_info->first[token_counter].token_type != TokenTypes::LEFT_PARENS )
				throw Error( "( Expected!", function_info->first[token_counter].line_number );
			
			ParseTree* lp = new ParseTree;
			lp->node.is_leaf = true;
			lp->node.token = function_info->first[token_counter];
			new_node->children.push_back( lp );
			token_counter++;

			// <expr>
			new_node->children.push_back( expr( ) );

			// )
			if( function_info->first[token_counter].token_type != TokenTypes::RIGHT_PARENS )
				throw Error( ") Expected!", function_info->first[token_counter].line_number );

			ParseTree* rp = new ParseTree;
			rp->node.is_leaf = true;
			rp->node.token = function_info->first[token_counter];
			new_node->children.push_back( rp );
			token_counter++;

			// <stmt>
			new_node->children.push_back( stmt( ) );
		}
		break;





	// begin<stmt list>end
	case TokenTypes::KW_BEGIN:
		{
			// begin
			ParseTree* beg = new ParseTree;
			beg->node.is_leaf = true;
			beg->node.token = function_info->first[token_counter];
			new_node->children.push_back( beg );
			token_counter++;

			// <stmt list>
			new_node->children.push_back( stmtList( ) );

			// end
			if( function_info->first[token_counter].token_type != TokenTypes::KW_END )
				throw Error( "end expected", function_info->first[token_counter].line_number );

			ParseTree* end = new ParseTree;
			end->node.is_leaf = true;
			end->node.token = function_info->first[token_counter];
			new_node->children.push_back( end );
			token_counter++;
		}
		break;




	// return <expr>;
	case TokenTypes::KW_RETURN:
		{
			// Return statement!!
			
			// return
			ParseTree* ret = new ParseTree;
			ret->node.is_leaf = true;
			ret->node.token = function_info->first[token_counter];
			new_node->children.push_back( ret );
			token_counter++;

			// <expr>
			new_node->children.push_back( expr( ) );

			// ;
			if( function_info->first[token_counter].token_type != TokenTypes::SEMI_COLON )
				throw Error( "Semi colon expected", function_info->first[token_counter].line_number );

			ParseTree* sem = new ParseTree;
			sem->node.is_leaf = true;
			sem->node.token = function_info->first[token_counter];
			new_node->children.push_back( sem );
			token_counter++;
		}
		break;






	case TokenTypes::SEMI_COLON:
		{
			// The empty statement!!
			ParseTree* sem = new ParseTree;
			sem->node.is_leaf = true;
			sem->node.token = function_info->first[token_counter];
			new_node->children.push_back( sem );
			token_counter++;
		}
		break;






	default:
		{
			// Either <expr> or <symbol> decl

			// Symbol declarations have id:<type> so we check for the colon, sneaky sneaky
			if( function_info->first[token_counter + 1].token_type == TokenTypes::COLON )
			{
				// <symbol decl>;
				new_node->children.push_back( symbolDecl( ) );
				
				// ;
				if( function_info->first[token_counter].token_type != TokenTypes::SEMI_COLON )
					throw Error( "Semi colon expected", function_info->first[token_counter].line_number );

				ParseTree* sem = new ParseTree;
				sem->node.is_leaf = true;
				sem->node.token = function_info->first[token_counter];
				new_node->children.push_back( sem );
				token_counter++;
			}



			else
			{
				// <expr>;

				new_node->children.push_back( expr( ) );

				// ;
				if( function_info->first[token_counter].token_type != TokenTypes::SEMI_COLON )
				{
					int a = 234;
					a++;
					a *= 234;
					throw Error( "Semi colon expected", function_info->first[token_counter].line_number );
				}

				ParseTree* sem = new ParseTree;
				sem->node.is_leaf = true;
				sem->node.token = function_info->first[token_counter];
				new_node->children.push_back( sem );
				token_counter++;
			}
		}

	} // end switch

	return new_node;
}





// <stmt list> -> <stmt><stmt list> | e
Parser::ParseTree* Parser::stmtList( )
{
	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::STMT_LIST;

	// if the next thing up is an end...it means that we have teh epsilon
	if( function_info->first[token_counter].token_type == TokenTypes::KW_END )
	{
		// we have got epsilon
		ParseTree* child = new ParseTree;
		child->node.is_leaf = true;
		child->node.token.token_type = TokenTypes::EPSILON;

		new_node->children.push_back( child );
	}
	else
	{
		new_node->children.push_back( stmt( ) );
		new_node->children.push_back( stmtList( ) );
	}

	return new_node;
}






// <expr> -> id<array of r>:=<expr> | <or expr>
Parser::ParseTree* Parser::expr( )
{
	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::EXPR;

	// we scan for := before a ; or unmatched ) or ]
	// I DONT want to have to implement backtracking
	int parens = 0;
	int bracks = 0;
	bool assign = false;
	for( unsigned int i=token_counter; i<function_info->first.size( ); i++ )
	{
		if( function_info->first[i].token_type == TokenTypes::ASSIGN )
		{
			// Do assignement!
			assign = true;
			break;
		}
		else if( function_info->first[i].token_type == TokenTypes::SEMI_COLON )
		{
			break;
		}
		else if( function_info->first[i].token_type == TokenTypes::LEFT_PARENS )
		{
			parens++;
		}
		else if( function_info->first[i].token_type == TokenTypes::RIGHT_PARENS )
		{
			parens--;
			if( parens < 0 )
			{
				break;
			}
		}
		else if( function_info->first[i].token_type == TokenTypes::LEFT_BRACKET )
		{
			bracks++;
		}
		else if( function_info->first[i].token_type == TokenTypes::RIGHT_BRACKET )
		{
			bracks--;
			if( bracks < 0 )
			{
				break;
			}
		}
	}

	// <expr> -> id<array of r>:=<expr> | <or expr>
	if( assign )
	{
		if( function_info->first[token_counter].token_type != TokenTypes::IDENTIFIER )
		{
			throw Error( "Only Identifiers can be assigned to", function_info->first[token_counter].line_number );
		}
		
		ParseTree* id = new ParseTree;
		id->node.is_leaf = true;
		id->node.token = function_info->first[token_counter];
		new_node->children.push_back( id );
		token_counter++;

		new_node->children.push_back( arrayOfR( ) );

		if( function_info->first[token_counter].token_type != TokenTypes::ASSIGN )
			throw Error( "Assignment not valid", function_info->first[token_counter].line_number );

		ParseTree* ass = new ParseTree;
		ass->node.is_leaf = true;
		ass->node.token = function_info->first[token_counter];
		new_node->children.push_back( ass );
		token_counter++;

		new_node->children.push_back( expr( ) );
	}
	else
	{
		new_node->children.push_back( orExpr( ) );
	}

	return new_node;
}






//<or expr> -> <and expr><or expr'>
Parser::ParseTree* Parser::orExpr( )
{
	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::OR_EXPR;

	new_node->children.push_back( andExpr( ) );
	new_node->children.push_back( orExprP( ) );

	return new_node;
}

// <or expr'> -> |<and expr><or expr'> | e
Parser::ParseTree* Parser::orExprP( )
{
	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::OR_EXPRP;

	if( function_info->first[token_counter].token_type == TokenTypes::OR )
	{
		ParseTree* or_n = new ParseTree;
		or_n->node.is_leaf = true;
		or_n->node.token = function_info->first[token_counter];
		new_node->children.push_back( or_n );
		token_counter++;

		new_node->children.push_back( andExpr( ) );
		new_node->children.push_back( orExprP( ) );
	}
	else
	{
		// epsilon choice!
		ParseTree* eps = new ParseTree;
		eps->node.is_leaf = true;
		eps->node.token.token_type = TokenTypes::EPSILON;
		new_node->children.push_back( eps );
	}

	return new_node;
}





// <and expr> -> <eq expr><and expr'>
Parser::ParseTree* Parser::andExpr( )
{
	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::AND_EXPR;

	new_node->children.push_back( eqExpr( ) );
	new_node->children.push_back( andExprP( ) );

	return new_node;
}

// <and expr'> -> &<eq expr><and expr'> | e
Parser::ParseTree* Parser::andExprP( )
{
	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::AND_EXPRP;

	if( function_info->first[token_counter].token_type == TokenTypes::AND )
	{
		ParseTree* and_n = new ParseTree;
		and_n->node.is_leaf = true;
		and_n->node.token = function_info->first[token_counter];
		new_node->children.push_back( and_n );
		token_counter++;

		new_node->children.push_back( eqExpr( ) );
		new_node->children.push_back( andExprP( ) );
	}
	else
	{
		// epsilon choice!
		ParseTree* eps = new ParseTree;
		eps->node.is_leaf = true;
		eps->node.token.token_type = TokenTypes::EPSILON;
		new_node->children.push_back( eps );
	}

	return new_node;
}




// <eq expr> -> <rel expr><eq expr'>
Parser::ParseTree* Parser::eqExpr( )
{
	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::EQ_EXPR;

	new_node->children.push_back( relExpr( ) );
	new_node->children.push_back( eqExprP( ) );

	return new_node;
}


// <eq expr'> -> ==<rel expr><eq expr'> | /=<rel expr><eq expr'> | e
Parser::ParseTree* Parser::eqExprP( )
{
	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::EQ_EXPRP;

	if( function_info->first[token_counter].token_type == TokenTypes::EQUALS )
	{
		ParseTree* eq = new ParseTree;
		eq->node.is_leaf = true;
		eq->node.token = function_info->first[token_counter];
		new_node->children.push_back( eq );
		token_counter++;

		new_node->children.push_back( relExpr( ) );
		new_node->children.push_back( eqExprP( ) );
	}
	else if( function_info->first[token_counter].token_type == TokenTypes::NOT_EQUALS )
	{
		ParseTree* ne = new ParseTree;
		ne->node.is_leaf = true;
		ne->node.token = function_info->first[token_counter];
		new_node->children.push_back( ne );
		token_counter++;

		new_node->children.push_back( relExpr( ) );
		new_node->children.push_back( eqExprP( ) );
	}
	else
	{
		// epsilon choice!
		ParseTree* eps = new ParseTree;
		eps->node.is_leaf = true;
		eps->node.token.token_type = TokenTypes::EPSILON;
		new_node->children.push_back( eps );
	}

	return new_node;
}




// <rel expr> -> <add expr><rel expr'>
Parser::ParseTree* Parser::relExpr( )
{
	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::REL_EXPR;

	new_node->children.push_back( addExpr( ) );
	new_node->children.push_back( relExprP( ) );

	return new_node;
}

//<rel expr'> -> <<add expr><rel expr'> | ><add expr><rel expr'> | <=<add expr><rel expr'> | >=<add expr><rel expr'> | e
Parser::ParseTree* Parser::relExprP( )
{
	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::REL_EXPRP;

	// I hate switches
	ParseTree* rel;
	ParseTree* eps;
	switch( function_info->first[token_counter].token_type )
	{
	case TokenTypes::LESS_THAN:
	case TokenTypes::GREATER_THAN:
	case TokenTypes::LESS_THAN_EQ:
	case TokenTypes::GREATER_THAN_EQ:
		rel = new ParseTree;
		rel->node.is_leaf = true;
		rel->node.token = function_info->first[token_counter];
		new_node->children.push_back( rel );
		token_counter++;
		new_node->children.push_back( addExpr( ) );
		new_node->children.push_back( relExprP( ) );
		break;

	default:
		// epsilon choice
		eps = new ParseTree;
		eps->node.is_leaf = true;
		eps->node.token.token_type = TokenTypes::EPSILON;
		new_node->children.push_back( eps );
		break;
	}

	return new_node;
}



// <add expr> -> <mult expr><add expr'>
Parser::ParseTree* Parser::addExpr( )
{
	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::ADD_EXPR;

	new_node->children.push_back( multExpr( ) );
	new_node->children.push_back( addExprP( ) );

	return new_node;
}

// <add expr'> -> +<mult expr><add expr'> | -<mult expr><add expr'> | e
Parser::ParseTree* Parser::addExprP( )
{
	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::ADD_EXPRP;

	if( (function_info->first[token_counter].token_type == TokenTypes::ADD) || (function_info->first[token_counter].token_type == TokenTypes::SUBTRACT) )
	{
		ParseTree* add = new ParseTree;
		add->node.is_leaf = true;
		add->node.token = function_info->first[token_counter];
		new_node->children.push_back( add );
		token_counter++;

		new_node->children.push_back( multExpr( ) );
		new_node->children.push_back( addExprP( ) );
	}
	else
	{
		// epsilon choice
		ParseTree* eps = new ParseTree;
		eps->node.is_leaf = true;
		eps->node.token.token_type = TokenTypes::EPSILON;
		new_node->children.push_back( eps );
	}

	return new_node;
}



// <mult expr> -> <pow expr><mult expr'>
Parser::ParseTree* Parser::multExpr( )
{
	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::MULT_EXPR;

	new_node->children.push_back( powExpr( ) );
	new_node->children.push_back( multExprP( ) );

	return new_node;
}

// <mult expr'> -> *<pow expr><mult expr'> | /<pow expr><mult expr'> | %<pow expr><mult expr'> | e
Parser::ParseTree* Parser::multExprP( )
{
	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::MULT_EXPRP;

	// I hate switches
	ParseTree* mult;
	ParseTree* eps;
	switch( function_info->first[token_counter].token_type )
	{
	case TokenTypes::MULTIPLY:
	case TokenTypes::DIVIDE:
	case TokenTypes::PERCENT:
		mult = new ParseTree;
		mult->node.is_leaf = true;
		mult->node.token = function_info->first[token_counter];
		new_node->children.push_back( mult );
		token_counter++;
		new_node->children.push_back( powExpr( ) );
		new_node->children.push_back( multExprP( ) );
		break;

	default:
		// epsilon choice
		eps = new ParseTree;
		eps->node.is_leaf = true;
		eps->node.token.token_type = TokenTypes::EPSILON;
		new_node->children.push_back( eps );
		break;
	}

	return new_node;
}





// Errr...the <pow expr> is a pain to parse
// unlike the other things above it is right-associative
// I will scan for the ^ token before a ; or unmatched ) or ]
// in the same way as done for :=

// <pow expr> -> <unary expr>^<pow expr> | <unary expr>
Parser::ParseTree* Parser::powExpr( )
{
	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::POW_EXPR;

	// we scan for ^ before a ; or unmatched ) or ]
	int parens = 0;
	int bracks = 0;
	bool expo = false;
	for( unsigned int i=token_counter; i<function_info->first.size( ); i++ )
	{
		if( function_info->first[i].token_type == TokenTypes::EXPONENT )
		{
			// Do exponential!
			expo = true;
			break;
		}
		else if( function_info->first[i].token_type == TokenTypes::SEMI_COLON )
		{
			break;
		}
		else if( function_info->first[i].token_type == TokenTypes::LEFT_PARENS )
		{
			parens++;
		}
		else if( function_info->first[i].token_type == TokenTypes::RIGHT_PARENS )
		{
			parens--;
			if( parens < 0 )
			{
				break;
			}
		}
		else if( function_info->first[i].token_type == TokenTypes::LEFT_BRACKET )
		{
			bracks++;
		}
		else if( function_info->first[i].token_type == TokenTypes::RIGHT_BRACKET )
		{
			bracks--;
			if( bracks < 0 )
			{
				break;
			}
		}
	}

	// <pow expr> -> <unary expr>^<pow expr>
	if( expo )
	{
		new_node->children.push_back( unaryExpr( ) );


		if( function_info->first[token_counter].token_type != TokenTypes::EXPONENT )
			throw Error( "Malformed Exponentiation", function_info->first[token_counter].line_number );
		ParseTree* exp = new ParseTree;
		exp->node.is_leaf = true;
		exp->node.token = function_info->first[token_counter];
		new_node->children.push_back( exp );
		token_counter++;


		new_node->children.push_back( powExpr( ) );
	}
	// <pow expr> -> <unary expr>
	else
	{
		new_node->children.push_back( unaryExpr( ) );
	}

	return new_node;
}



// <unary expr> -> !<unary expr> | -<unary expr> | +<unary expr> | <term expr>
Parser::ParseTree* Parser::unaryExpr( )
{
	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::UNARY_EXPR;


	ParseTree* una;
	switch( function_info->first[token_counter].token_type )
	{
	case TokenTypes::NOT:
	case TokenTypes::SUBTRACT:		// these are unary + and -
	case TokenTypes::ADD:
		
		// The first three rules
		una = new ParseTree;
		una->node.is_leaf = true;
		una->node.token = function_info->first[token_counter];
		new_node->children.push_back( una );
		token_counter++;

		new_node->children.push_back( unaryExpr( ) );
		break;



	default:
		// the <term expr> rule
		new_node->children.push_back( termExpr( ) );
		break;
	}


	return new_node;
}



// <term expr> -> (<expr>) | id<array of r> | literal | <func call>
Parser::ParseTree* Parser::termExpr( )
{
	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::TERM_EXPR;


	Token curr = function_info->first[token_counter];

	switch( function_info->first[token_counter].token_type )
	{
	case TokenTypes::LEFT_PARENS:
		{
			// (<expr>)
			ParseTree* lp = new ParseTree;
			lp->node.is_leaf = true;
			lp->node.token = function_info->first[token_counter];
			new_node->children.push_back( lp );
			token_counter++;

			new_node->children.push_back( expr( ) );

			if( function_info->first[token_counter].token_type != TokenTypes::RIGHT_PARENS )
				throw Error( ") Expected", function_info->first[token_counter].line_number );
		
			ParseTree* rp = new ParseTree;
			rp->node.is_leaf = true;
			rp->node.token = function_info->first[token_counter];
			new_node->children.push_back( rp );
			token_counter++;
		}
		break;


		

	case TokenTypes::INT_LITERAL:
	case TokenTypes::REAL_LITERAL:
	case TokenTypes::BOOL_LITERAL:
	case TokenTypes::CHAR_LITERAL:
		{
			ParseTree* lit = new ParseTree;
			lit->node.is_leaf = true;
			lit->node.token = function_info->first[token_counter];
			new_node->children.push_back( lit );
			token_counter++;
		}
		break;



	case TokenTypes::IDENTIFIER:
		{
			if( function_info->first[token_counter + 1].token_type == TokenTypes::LEFT_PARENS )
			{
				// <func call>
				new_node->children.push_back( funcCall( ) );
			}
			else
			{
				// id<array of r>
				ParseTree* id = new ParseTree;
				id->node.is_leaf = true;
				id->node.token = function_info->first[token_counter];
				new_node->children.push_back( id );
				token_counter++;

				new_node->children.push_back( arrayOfR( ) );
			}
		}
		break;

	default:
		throw Error( "Malformed expression", function_info->first[token_counter].line_number );
	}


	return new_node;
}





// <func call> -> id(<params>)
Parser::ParseTree* Parser::funcCall( )
{
	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::FUNC_CALL;

	// id
	if( function_info->first[token_counter].token_type != TokenTypes::IDENTIFIER )
		throw Error( "Identifier expected", function_info->first[token_counter].line_number );

	ParseTree* id = new ParseTree;
	id->node.is_leaf = true;
	id->node.token = function_info->first[token_counter];
	new_node->children.push_back( id );
	token_counter++;

	// (
	if( function_info->first[token_counter].token_type != TokenTypes::LEFT_PARENS )
		throw Error( "( Expected", function_info->first[token_counter].line_number );

	ParseTree* lp = new ParseTree;
	lp->node.is_leaf = true;
	lp->node.token = function_info->first[token_counter];
	new_node->children.push_back( lp );
	token_counter++;

	// <params>
	new_node->children.push_back( params( ) );

	// )
	if( function_info->first[token_counter].token_type != TokenTypes::RIGHT_PARENS )
		throw Error( ") Expected", function_info->first[token_counter].line_number );

	ParseTree* rp = new ParseTree;
	rp->node.is_leaf = true;
	rp->node.token = function_info->first[token_counter];
	new_node->children.push_back( rp );
	token_counter++;

	return new_node;
}





// <params> -> <expr><next param> | e
Parser::ParseTree* Parser::params( )
{
	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::PARAMS;

	// if next token is a ), we choose epsilon
	if( function_info->first[token_counter].token_type == TokenTypes::RIGHT_PARENS )
	{
		ParseTree* epsilon = new ParseTree;
		epsilon->node.is_leaf = true;
		epsilon->node.token.token_type = TokenTypes::EPSILON;
		new_node->children.push_back( epsilon );
	}
	else
	{
		new_node->children.push_back( expr( ) );
		new_node->children.push_back( nextParam( ) );
	}

	return new_node;
}




// <next param> -> ,<expr><next param>  | e
Parser::ParseTree* Parser::nextParam( )
{
	ParseTree* new_node = new ParseTree;
	new_node->node.is_leaf = false;
	new_node->node.non_term = NonTerminals::NEXT_PARAMS;

	// Check for the comma
	if( function_info->first[token_counter].token_type == TokenTypes::COMMA )
	{
		ParseTree* com = new ParseTree;
		com->node.is_leaf = true;
		com->node.token = function_info->first[token_counter];
		new_node->children.push_back( com );
		token_counter++;

		new_node->children.push_back( expr( ) );
		new_node->children.push_back( nextParam( ) );
	}
	else
	{
		ParseTree* epsilon = new ParseTree;
		epsilon->node.is_leaf = true;
		epsilon->node.token.token_type = TokenTypes::EPSILON;
		new_node->children.push_back( epsilon );
	}

	return new_node;
}






// for debugging
std::ostream& operator<<( std::ostream& os, const NonTerminals::NonTerminal& nt )
{
	using namespace NonTerminals;

	switch( nt )
	{
	case FUNCTION: os << "FUNCTION"; break;
	case SYMBOL_DECL: os << "SYMBOL_DECL"; break;
	case FORMAL_ARGS: os << "FORMAL_ARGS"; break;
	case NEXT_FORMAL_ARG: os << "NEXT_FORMAL_ARG"; break;
	case TYPE: os << "TYPE"; break;
	case BASE_TYPE: os << "BASE_TYPE"; break;
	case ARRAY_OF_C: os << "ARRAY_OF_C"; break;
	case ARRAY_OF_R: os << "ARRAY_OF_R"; break;
	case STMT: os << "STMT"; break;
	case STMT_LIST: os << "STMT_LIST"; break;
	case EXPR: os << "EXPR"; break;
	case OR_EXPR: os << "OR_EXPR"; break;
	case OR_EXPRP: os << "OR_EXPRP"; break;
	case AND_EXPR: os << "AND_EXPR"; break;
	case AND_EXPRP: os << "AND_EXPRP"; break;
	case EQ_EXPR: os << "EQ_EXPR"; break;
	case EQ_EXPRP: os << "EQ_EXPRP"; break;
	case REL_EXPR: os << "REL_EXPR"; break;
	case REL_EXPRP: os << "REL_EXPRP"; break;
	case ADD_EXPR: os << "ADD_EXPR"; break;
	case ADD_EXPRP: os << "ADD_EXPRP"; break;
	case MULT_EXPR: os << "MULT_EXPR"; break;
	case MULT_EXPRP: os << "MULT_EXPRP"; break;
	case POW_EXPR: os << "POW_EXPR"; break;
	case UNARY_EXPR: os << "UNARY_EXPR"; break;
	case TERM_EXPR: os << "TERM_EXPR"; break;
	case FUNC_CALL: os << "FUNC_CALL"; break;
	case PARAMS: os << "PARAMS"; break;
	case NEXT_PARAMS: os << "NEXT_PARAMS"; break;
	}

	return os;
}


// for debugging -  a full print (including nonterminals and what they expand to)
void printLeftmostDerivation( Parser::ParseTree* tree, std::ostream& os )
{
	if( !tree->node.is_leaf )
	{
		// Print the non terminal
		os << "Non-Terminal: " << tree->node.non_term << "\n\t";

		// Print all of it's children...
		for( unsigned int i=0; i<tree->children.size( ); i++ )
		{
			if( tree->children[i]->node.is_leaf )
				os << tree->children[i]->node.token.lexeme << " ";
			else
				os << tree->children[i]->node.non_term << " ";
		}

		os << "\n\n";

		// recurse
		for( unsigned int i=0; i<tree->children.size( ); i++ )
		{
			printLeftmostDerivation( tree->children[i], os );
		}
	}
}


// prints just the terminals on the leftmost traversal
// This should recreate the input text!
void printLeftmostTrace( Parser::ParseTree* tree, std::ostream& os )
{
	if( tree->node.is_leaf )
	{
		if( tree->node.token.token_type != TokenTypes::EPSILON )
			os << tree->node.token.lexeme << " ";

		// Newlines after semi colons or begin...
		if( (tree->node.token.token_type == TokenTypes::SEMI_COLON) || (tree->node.token.token_type == TokenTypes::KW_END) )
			os << "\n";
	}
	else
	{
		// Print kids
		for( unsigned int i=0; i<tree->children.size( ); i++ )
		{
			printLeftmostTrace( tree->children[i], os );
		}
	}

}








