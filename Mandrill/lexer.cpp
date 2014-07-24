// lexer.cpp
// Implements lexical analysis

#include <algorithm>

#include "lexer.hpp"
#include "errors.hpp"


Lexer::Lexer( std::istream* input )
{
	if( !input )
		throw Error( "Input Could Not Be Read" );		// Should not happen...

	instream = input;
}

// This function reads a character from the input stream
// And updates the line counter accordingly.
int line = 1;
char getAChar( std::istream* is )
{
	char a = static_cast<char>( is->get( ) );

	if( a == '\n' )
		line++;

	return a;
}

// this function reads an input char, skipping over comments
char getInputIgnoreComments( std::istream* is )
{
	char a = getAChar( is );

	if( a == '#' )	// beginning of a comment
	{
		a = getAChar( is );

		// scan until end of comment
		while( a != '#' )
		{
			a = getAChar( is );
			if( is->eof( ) )
				throw Error( "End of file reached before end of comment", line );
		}

		// we use recursion to ensure that there's not *Another* comment
		return getInputIgnoreComments( is );
	}
	else
	{
		return a;
	}
}

// This function returns a word token - that
// is an identifier or a keyword
Token Lexer::beginWord( char first )
{
	Token token;

	while( isalpha(first) || isdigit(first) || (first == '_') )
	{
		token.lexeme.push_back( first );

		first = getInputIgnoreComments( instream );
	}

	if( first != '\n' )
		instream->putback( first );	// putback whatever was not part of this word!

	// Check for the reserved words...
	if( token.lexeme == "int" )
		token.token_type = TokenTypes::KW_INT;
	else if( token.lexeme == "real" )
		token.token_type = TokenTypes::KW_REAL;
	else if( token.lexeme == "char" )
		token.token_type = TokenTypes::KW_CHAR;
	else if( token.lexeme == "bool" )
		token.token_type = TokenTypes::KW_BOOL;
	
	else if( token.lexeme == "if" )
		token.token_type = TokenTypes::KW_IF;
	else if( token.lexeme == "else" )
		token.token_type = TokenTypes::KW_ELSE;
	else if( token.lexeme == "begin" )
		token.token_type = TokenTypes::KW_BEGIN;
	else if( token.lexeme == "end" )
		token.token_type = TokenTypes::KW_END;
	else if( token.lexeme == "while" )
		token.token_type = TokenTypes::KW_WHILE;
	else if( token.lexeme == "return" )
		token.token_type = TokenTypes::KW_RETURN;
	
	else if( token.lexeme == "true" )
		token.token_type = TokenTypes::BOOL_LITERAL;
	else if( token.lexeme == "false" )
		token.token_type = TokenTypes::BOOL_LITERAL;

	else
		token.token_type = TokenTypes::IDENTIFIER;

	return token;
}

// This function returns a word token - either an
// int or real
Token Lexer::beginNumber( char first )
{
	Token token;

	while( isdigit(first) || (first == '.') )
	{
		token.lexeme.push_back( first );

		first = getInputIgnoreComments( instream );
	}

	if( first != '\n' )
		instream->putback( first );		// not part of *this* lexeme

	// switch on how many decimal points there are
	switch( std::count( token.lexeme.begin( ), token.lexeme.end( ), '.' ) )
	{
	case 0:
		token.token_type = TokenTypes::INT_LITERAL;
		break;

	case 1:
		token.token_type = TokenTypes::REAL_LITERAL;
		break;

	default:
		throw Error( "Lexical Error - Too Many Decimals", line );
	}

	return token;
}

// This function returns all of the punctuation type tokens
// including character literals 'a'
Token Lexer::beginPunct( char first )
{
	Token token;
	token.lexeme.push_back( first );

	char next;

	switch( first )
	{
	case '+':
		token.token_type = TokenTypes::ADD;
		break;

	case '-':
		token.token_type = TokenTypes::SUBTRACT;
		break;

	case '*':
		token.token_type = TokenTypes::MULTIPLY;
		break;
		
	case '^':
		token.token_type = TokenTypes::EXPONENT;
		break;

	case '%':
		token.token_type = TokenTypes::PERCENT;
		break;

	case '&':
		token.token_type = TokenTypes::AND;
		break;

	case '|':
		token.token_type = TokenTypes::OR;
		break;

	case '!':
		token.token_type = TokenTypes::NOT;
		break;

	case '[':
		token.token_type = TokenTypes::LEFT_BRACKET;
		break;

	case ']':
		token.token_type = TokenTypes::RIGHT_BRACKET;
		break;

	case '(':
		token.token_type = TokenTypes::LEFT_PARENS;
		break;

	case ')':
		token.token_type = TokenTypes::RIGHT_PARENS;
		break;

	case ';':
		token.token_type = TokenTypes::SEMI_COLON;
		break;

	case ',':
		token.token_type = TokenTypes::COMMA;
		break;

	case '/':
		// we check for / or /=
		next = getInputIgnoreComments( instream );
		if(  next == '=' )
		{
			token.lexeme.push_back( next );
			token.token_type = TokenTypes::NOT_EQUALS;
		}
		else
		{
			// oops
			if( next != '\n' )
				instream->putback( next );
			token.token_type = TokenTypes::DIVIDE;
		}
		break;

	case '=':
		next = getInputIgnoreComments( instream );
		if( next == '=' )
		{
			token.lexeme.push_back( next );
			token.token_type = TokenTypes::EQUALS;
		}
		else
		{
			// There is no plain =
			throw Error( "= Is Not A Valid Operator", line );
		}
		break;

	case ':':	// looks weird :)
		next = getInputIgnoreComments( instream );
		if( next == '=' )
		{
			token.lexeme.push_back( next );
			token.token_type = TokenTypes::ASSIGN;
		}
		else
		{
			if( next != '\n' )
				instream->putback( next );
			token.token_type = TokenTypes::COLON;
		}
		break;

	case '<':
		next = getInputIgnoreComments( instream );
		if( next == '=' )
		{
			token.lexeme.push_back( next );
			token.token_type = TokenTypes::LESS_THAN_EQ;
		}
		else
		{
			if( next != '\n' )
				instream->putback( next );
			token.token_type = TokenTypes::LESS_THAN;
		}
		break;

	case '>':
		next = getInputIgnoreComments( instream );
		if( next == '=' )
		{
			token.lexeme.push_back( next );
			token.token_type = TokenTypes::GREATER_THAN_EQ;
		}
		else
		{
			if( next != '\n' )
				instream->putback( next );
			token.token_type = TokenTypes::GREATER_THAN;
		}
		break;

	case '\'':		// a '
		next = getInputIgnoreComments( instream );
		token.lexeme.push_back( next );
		next = getInputIgnoreComments( instream );
		if( next != '\'' )
			throw Error( "Character Literals Can Only Be One Character", line );
		token.lexeme.push_back( next );
		token.token_type = TokenTypes::CHAR_LITERAL;
		break;

	default:
		std::string msg = "Invalid Symbol: ";
		msg.push_back( first );
		throw Error( msg, line );
	}

	return token;
}

// This function gets the next input token
Token Lexer::getNext( )
{
	char next_char = getInputIgnoreComments( instream );
	
	// Test for end of input file!
	if( instream->eof( ) )
	{
		Token eof;
		eof.token_type = TokenTypes::EPSILON;
		return eof;
	}

	// skip over white space
	while( (next_char == ' ') || (next_char == '\n') || (next_char == '\t') )
	{
		next_char = getInputIgnoreComments( instream );
	}

	Token nextone;
	if( isalpha(next_char) )
	{
		nextone = beginWord( next_char );	//word
	}
	else if( isdigit(next_char) )
	{
		nextone = beginNumber( next_char );	// number
	}
	else if( isprint(next_char) )	// punctuation
	{
		nextone = beginPunct( next_char );
	}
	else	// Some weird control character...
	{
		return getNext( );
	}

	// Set the line counter. This sometimes is one more than the line
	// the token actually appeared on, but that is no big deal.
	nextone.line_number = line;
	return nextone;
}

// this function returns all of the remaining tokens in a vector
std::vector<Token> Lexer::getAll( )
{
	std::vector<Token> tokens;

	Token next = getNext( );
	while( next.token_type != TokenTypes::EPSILON )
	{
		tokens.push_back( next );
		next = getNext( );
	}

	return tokens;
}



// The spiltIntoFunctions function
std::vector<Function> splitIntoFunctions( const std::vector<Token>& tokens )
{
	std::vector<Function> functions;
	Function function;

	int begins = 0;
	bool had_a_begin = false;

	// for each token...
	for( unsigned int i=0; i<tokens.size( ); i++ )
	{
		// Add the token
		function.first.push_back( tokens[i] );

		if( tokens[i].token_type == TokenTypes::KW_BEGIN )
		{
			had_a_begin = true;
			begins++;
		}
		else if( tokens[i].token_type == TokenTypes::KW_END )
		{
			begins--;

			// More ends than begins...
			if( begins < 0 )
				throw Error( "End not expected", tokens[i].line_number );
		}

		// if we hit a semi-colon before a begin, then the function was a one-liner!
		else if( (tokens[i].token_type == TokenTypes::SEMI_COLON) && !had_a_begin )
		{
			functions.push_back( function );

			function.first.clear( );
			begins = 0;
			had_a_begin = false;
		}

		if( (begins == 0) && had_a_begin )	// function is over!
		{
			functions.push_back( function );

			function.first.clear( );
			begins = 0;
			had_a_begin = false;
		}
	}

	if( begins > 0 )
		throw Error( "Input ended before final end" );

	return functions;
}





// This function prints out a Token - mostly for debugging
std::ostream& operator<<( std::ostream& os, const TokenTypes::TokenType& t )
{
	using namespace TokenTypes;

	switch( t )
	{
	case KW_INT: os << "KW_INT"; break;
	case KW_REAL: os << "KW_REAL"; break;
	case KW_CHAR: os << "KW_CHAR"; break;
	case KW_BOOL: os << "KW_BOOL"; break;
	case KW_IF: os << "KW_IF"; break;
	case KW_ELSE: os << "KW_ELSE"; break;
	case KW_BEGIN: os << "KW_BEGIN"; break;
	case KW_END: os << "KW_END"; break;
	case KW_WHILE: os << "KW_WHILE"; break;
	case KW_RETURN: os << "KW_RETURN"; break;

	case ADD: os << "ADD"; break;
	case SUBTRACT: os << "SUBTRACT"; break;
	case DIVIDE: os << "DIVIDE"; break;
	case MULTIPLY: os << "MULTIPLY"; break;
	case EXPONENT: os << "EXPONENT"; break;
	case PERCENT: os << "PERCENT"; break;
	case AND: os << "AND"; break;
	case OR: os << "OR"; break;
	case NOT: os << "NOT"; break;

	case LEFT_BRACKET: os << "LEFT_BRACKET"; break;
	case RIGHT_BRACKET: os << "RIGHT_BRACKET"; break;
	case LEFT_PARENS: os << "LEFT_PARENS"; break;
	case RIGHT_PARENS: os << "RIGHT_PARENS"; break;
	
	case LESS_THAN: os << "LESS_THAN"; break;
	case GREATER_THAN: os << "GREATER_THAN"; break;
	case LESS_THAN_EQ: os << "LESS_THAN_EQ"; break;
	case GREATER_THAN_EQ: os << "GREATER_THAN_EQ"; break;
	case EQUALS: os << "EQUALS"; break;
	case NOT_EQUALS: os << "NOT_EQUALS"; break;

	case ASSIGN: os << "ASSIGN"; break;
	case COLON: os << "COLON"; break;
	case SEMI_COLON: os << "SEMI_COLON"; break;
	case COMMA: os << "COMMA"; break;

	case IDENTIFIER: os << "IDENTIFIER"; break;
	case INT_LITERAL: os << "INT_LITERAL"; break;
	case REAL_LITERAL: os << "REAL_LITERAL"; break;
	case CHAR_LITERAL: os << "CHAR_LITERAL"; break;
	case BOOL_LITERAL: os << "BOOL_LITERAL"; break;

	case EPSILON: os << "EPSILON"; break;
	}

	return os;
}

