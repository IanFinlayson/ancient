// main.cpp
// contains the main function

#include <iostream>
#include <iomanip>
#include <fstream>

#include "lexer.hpp"
#include "symbol.hpp"
#include "errors.hpp"
#include "parse.hpp"
#include "codegen.hpp"
#include "assem.hpp"

int main( int argc, char** argv )
{
	// check for correct use: "mandrill file.mnd out.a"
	// if out.a is ommmitted, default will be out.a
	if( argc < 2 )
	{
		std::cout << "Usage:\nmandrill infile [outfile]" << std::endl;
		return 0;
	}

	// Compiler Errors are reported through exceptions
	try
	{
		// Setup file names
		std::string input_file_name = argv[1];
		std::string output_file_name = (argc >= 3) ? argv[2] : "out.s";

		// Open the input file
		std::ifstream infile( input_file_name.c_str( ) );
		if( !infile.is_open( ) )
			throw Error( "Input File Could Not Be Opened" );


		// Create the lexer
		Lexer lexer( &infile );


		// Get a vector of all the tokens in the input
		std::vector<Token> tokens = lexer.getAll( );
		


		// spit the tokens into functions
		std::vector<Function> functions = splitIntoFunctions( tokens );

		// Loop throught the functions and creat a parse tree for each one!
		std::vector<Parser::ParseTree*> parse_trees;
		for( unsigned int i=0; i<functions.size( ); i++ )
		{
			Parser parser( &functions[i] );
			parse_trees.push_back( parser.function( ) );
		}

		// Now we should have a parse tree for each function!
		// The parse trees are in the vector parse_trees
		// the (empty) symbol tables are in the functions[i]->second thing
		// we pull them out...
		std::vector<SymbolTable> symbol_tables;
		for( unsigned int i=0; i<functions.size( ); i++ )
		{
			symbol_tables.push_back( functions[i].second );
		}


		// Populate the symbol table and add type info...
		for( unsigned int i=0; i<parse_trees.size( ); i++ )
		{
			addTypes( symbol_tables[i], parse_trees[i] );

			// That function includes the name of the function, but we want that removed...
			// The symbol_tables vector is for local variables.
			// There is also a symbol table going to be created just for functions...

			std::string func_name = parse_trees[i]->children[0]->children[0]->node.token.lexeme;
			symbol_tables[i].erase( func_name );
		}

		// Now we make the symbol table for functions
		SymbolTable function_symbols;
		for( unsigned int i=0; i<parse_trees.size( ); i++ )
		{
			addFunctionToTable( function_symbols, parse_trees[i] );
		}

		// add std library functions
		addStdLib( function_symbols );

		// Now we have the parse_trees, symbol_tables, and function_symbols all ready to go.
		// The next step is type checking. There is no output from this, we just will have an exception
		// thrown if there are errors discovered. We type check each function separately.
		for( unsigned int i=0; i<parse_trees.size( ); ++i )
		{
			checkTypes( parse_trees[i], symbol_tables[i], function_symbols, parse_trees[i]->children[0]->children[0]->node.token.lexeme );
		}


		// Now we send this to the intermediate code generator (they add to the vector!)
		std::vector<Instruct> inter_code;
		for( unsigned int i=0; i<parse_trees.size( ); i++ )
		{
			genFunc( inter_code, parse_trees[i], symbol_tables[i], function_symbols );
		}


		// TEMPORARY - SAVE INTERMEDIATE CODE
		std::ofstream temp( (output_file_name + ".temp.txt").c_str( ) );
		for( unsigned int i=0; i<inter_code.size( ); i++ )
		{
			inter_code[i].print( temp );
		}
		temp.close( );


		// Now we generate assembly from the intermeidate code
		genAssem( inter_code, output_file_name, symbol_tables, function_symbols );
	}
	catch( Error& error )	// Report Compiler Errors
	{
		std::cout << error.what( ) << std::endl;
	}


	return 0;
}

