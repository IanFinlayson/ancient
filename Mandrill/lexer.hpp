// lexer.hpp
// contains the lexer

#ifndef MAN_LEX_HPP
#define MAN_LEX_HPP

#include <iostream>
#include <vector>
#include <string>

#include "symbol.hpp"

// It's in a namespace to prevent name cluttering!
namespace TokenTypes
{
	// A token type can be any of these
	enum TokenType
	{
		KW_INT,
		KW_REAL,
		KW_CHAR,
		KW_BOOL,
		KW_IF,
		KW_ELSE,
		KW_BEGIN,
		KW_END,
		KW_WHILE,
		KW_RETURN,

		ADD,
		SUBTRACT,
		DIVIDE,
		MULTIPLY,
		EXPONENT,
		PERCENT,
		AND,
		OR,
		NOT,

		LEFT_BRACKET,
		RIGHT_BRACKET,
		LEFT_PARENS,
		RIGHT_PARENS,
		
		LESS_THAN,
		GREATER_THAN,
		LESS_THAN_EQ,
		GREATER_THAN_EQ,
		EQUALS,
		NOT_EQUALS,

		ASSIGN,
		COLON,
		SEMI_COLON,
		COMMA,

		IDENTIFIER,
		INT_LITERAL,
		REAL_LITERAL,
		CHAR_LITERAL,
		BOOL_LITERAL,

		EPSILON		// Used to signal EOF or empty
	};
}

// Function to allow us to print the token types (mostly for debugging)
std::ostream& operator<<( std::ostream& os, const TokenTypes::TokenType& t );


// A token consists of the type and the lexeme and a line number
struct Token
{
	TokenTypes::TokenType token_type;
	std::string lexeme;
	unsigned int line_number;
};


// The lexer class is constructed from an input stream and
// a symbol table.
class Lexer
{
public:
	Lexer( std::istream* input );
	std::vector<Token> getAll( );

private:
	std::istream* instream;

	Token getNext( );

	Token beginWord( char first );
	Token beginNumber( char first );
	Token beginPunct( char first );
};

// This function takes the vector of tokens and spilts it into
// sperate vectors along functions. It also returns a symbol
// table for each function. THis allows the compiler to only deal
// with a function at a time as it's compiling.
typedef std::pair<std::vector<Token>, SymbolTable> Function;

std::vector<Function> splitIntoFunctions( const std::vector<Token>& tokens );

#endif



