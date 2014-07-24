// codegen.hpp
// generates intermidate code


#ifndef MAN_CODEGEN_H
#define MAN_CODEGEN_H

#include "symbol.hpp"
#include "parse.hpp"

namespace InterInstrs
{
	enum InterInstr
	{
		MOVE = 0,	// MOVE A B			(A = B)
		NOT,		// NOT A B			(A = !B)

		ADD,		// ADD A B C		(A = B + C)
		ADD_S,

		MULT,		// MULT A B C		(A = B * C)
		MULT_S,

		SUB,		// SUB A B C		(A = B - C)
		SUB_S,

		DIV,		// DIV A B C		(A = B / C)
		DIV_S,

		REM,		// REM A B C		(A = B % C)

		AND,		// AND A B C		(A = B & C)
		OR,			// OR A B C			(A = B | C)

		POW,		// POW A B C		(A = B ^ C)
		POW_S,

		LT,			// LT A B C			(A = B < C)	(1 or 0)
		LT_S,

		LTE,
		LTE_S,

		GT,
		GT_S,

		GTE,
		GTE_S,

		EQ,
		EQ_S,

		NE,
		NE_S,

		LABEL,		// LABEL L			(mark next instruction with a label)
		GOTO,		// GOTO L			(jumps to L)
		IF,			// IF X L			(if X is true, jump to L)

		FUNCTION,	// FUNCTION L		(Begin a new function labeled L)
		PARAM,		// PARAM A			(Set A as a parameter when CALLING a function)
		ARG,		// ARG A			(Declare A as a parameter when DEFINING a function)
		CALL,		// CALL F N			(Call F with N parameters)
		LOCAL,		// LOCAL A			(Create local variable A)
		RETURN,		// RETURN A			(returns A)
		GETRET,		// GETRET A			(gets the return value from a func ($ra) and puts it into A)

		INDEXL,		// INDEXL X Y I		(X = Y[I])
		INDEXR		// INDEXR X I Y		(X[I] = Y)
	};
}


// Used in Instruct objects (represents a SymbolTable entry OR a literal)
struct Address
{
public:
	bool literal, valid;

	// If literal
	Type::BaseType lit_type;
	std::string lit;

	// If not literal
	SymbolTable::iterator address;
	Address* offset;

	Address( SymbolTable::iterator addr )
	{
		literal = false;
		valid = true;
		address = addr;
		offset = NULL;
	}
	Address( Type::BaseType type, const std::string& literal_string )
	{
		literal = true;
		valid = true;
		lit_type = type;
		lit = literal_string;
		offset = NULL;
	}
	Address( )
	{
		// NULL Address
		valid = false;
	}
	bool isValid( ){ return valid; }
	void print( std::ostream& os );
	void setOffset( Address* off_set ){ offset=off_set; }
	std::string getLex( )
	{
		if( literal ) return lit;
		else return address->second.lexeme;

	}
	bool operator==( const Address& rhs ) const
	{
		return (literal == rhs.literal) && (lit_type == rhs.lit_type) && (lit == rhs.lit)
			&& (address == rhs.address) && (offset == rhs.offset);

	}
};




// Some of these fields are not used based on the op!
struct Instruct
{
public:
	// Used for all
	InterInstrs::InterInstr op;

	// The arguments (temps are inserted!)
	Address arg1, arg2, result;

	// Used for LABEL, GOTO, and IF*
	std::string label;

	// Used for CALL
	int arg_count;

	Instruct( InterInstrs::InterInstr op, Address arg1=Address( ), Address arg2=Address( ), Address result=Address( ),
		const std::string& label="", int arg_count=-1 );

	void print( std::ostream& os );
};


// Generate code for a function and store in code vector
void genFunc( std::vector<Instruct>& code, Parser::ParseTree* func, SymbolTable& locals, SymbolTable& functions );

#endif













