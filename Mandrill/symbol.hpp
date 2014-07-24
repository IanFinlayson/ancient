// symbol.hpp
// contains symbol table information

#ifndef MAN_SYMB_HPP
#define MAN_SYMB_HPP

#include <string>
#include <map>
#include <vector>


// Represents a Mandrill data type
struct Type
{
	enum BaseType
	{
		INT,
		REAL,
		CHAR,
		BOOL
	};

	// The base type (return value for functions)
	BaseType base;

	// Array dimension is how many [][][]
	int array_dimension;

	// all of the sizes eg [12][31][23]
	std::vector<int> array_sizes;

	// For functions only - parameter types
	bool function;
	std::vector<Type> parameters;
	std::vector<std::string> param_names;	// hack
};

std::ostream& operator<<( std::ostream& os, const Type::BaseType& t );


// Represents an entry in the symbol table - will be added to as needed
struct SymbolEntry
{
	std::string lexeme;
    Type type;

	int line_number;


	bool reg;		// if reg, use reg_num. else f_p_o
	int reg_num;
	int frame_pointer_offset;

	int float_rg_num;
};

// The table maps lexemes to symbols
typedef std::map<std::string, SymbolEntry> SymbolTable;

void printTable( const SymbolTable& table, std::ostream& os );

#endif





