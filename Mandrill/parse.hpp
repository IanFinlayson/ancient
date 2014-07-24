// parse.hpp
// building a parse tree from the tokens

#ifndef MAN_PARSE_HPP
#define MAN_PARSE_HPP

#include "lexer.hpp"

// All of the non-terminals
// (in a namespace to not pollute ::)
namespace NonTerminals
{
	enum NonTerminal
	{
		FUNCTION,
		SYMBOL_DECL,
		FORMAL_ARGS,
		NEXT_FORMAL_ARG,
		TYPE,
		BASE_TYPE,
		ARRAY_OF_C,
		ARRAY_OF_R,
		STMT,
		STMT_LIST,
		EXPR,
		OR_EXPR,
		OR_EXPRP,
		AND_EXPR,
		AND_EXPRP,
		EQ_EXPR,
		EQ_EXPRP,
		REL_EXPR,
		REL_EXPRP,
		ADD_EXPR,
		ADD_EXPRP,
		MULT_EXPR,
		MULT_EXPRP,
		POW_EXPR,
		UNARY_EXPR,
		TERM_EXPR,
		FUNC_CALL,
		PARAMS,
		NEXT_PARAMS
	};
}

// function to print a non terminal
std::ostream& operator<<( std::ostream& os, const NonTerminals::NonTerminal& nt );


// Contains the parsing code
class Parser
{
public:
	Parser( Function* this_function ) : function_info(this_function), token_counter(0) {}

	// A ParseTree node
	struct Node
	{
		NonTerminals::NonTerminal non_term;
		Token token;

		bool is_leaf;	// if is_leaf, then token is valid, else non_term!!!
	};

	// A parse tree
	struct ParseTree
	{
		Node node;
		std::vector<ParseTree*> children;
	};

	// All of these non-terminal functions return ParseTree*
	// Each of these functions expect that token_counter will
	// be set to the token they're supposed to be starting with

	// function is the only one that should be called by calling code
	ParseTree* function( );

private:
	ParseTree* symbolDecl( );
	ParseTree* formalArgs( );
	ParseTree* nextFormalArg( );

	ParseTree* type( );
	ParseTree* baseType( );
	ParseTree* arrayOfC( );
	ParseTree* arrayOfR( );

	ParseTree* stmt( );
	ParseTree* stmtList( );

	ParseTree* expr( );

	ParseTree* orExpr( );
	ParseTree* orExprP( );

	ParseTree* andExpr( );
	ParseTree* andExprP( );

	ParseTree* eqExpr( );
	ParseTree* eqExprP( );

	ParseTree* relExpr( );
	ParseTree* relExprP( );

	ParseTree* addExpr( );
	ParseTree* addExprP( );

	ParseTree* multExpr( );
	ParseTree* multExprP( );

	ParseTree* powExpr( );
	ParseTree* unaryExpr( );
	ParseTree* termExpr( );

	ParseTree* funcCall( );
	ParseTree* params( );
	ParseTree* nextParam( );

	// Contains the tokens and the symbol table
	Function* function_info;
	int token_counter;
};


// Function to print a leftmost derivation from a parse tree
// for debugging purposes
void printLeftmostDerivation( Parser::ParseTree* tree, std::ostream& os );

// Prints just the terminals on a left-most trace - should be the same as the program text!
void printLeftmostTrace( Parser::ParseTree* tree, std::ostream& os );

// this function adds type info to the table
void addTypes( SymbolTable& table, Parser::ParseTree* tree );

// this function adds a function to a symbol table (with types incl. params)
void addFunctionToTable( SymbolTable& func_table, Parser::ParseTree* tree );

void addStdLib( SymbolTable& func_table );

// this function does type checking
void checkTypes( Parser::ParseTree* tree, const SymbolTable& locals, const SymbolTable& functions, const std::string& func_name  );

#endif

