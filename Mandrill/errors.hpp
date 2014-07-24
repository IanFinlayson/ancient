// errors.hpp
// compiler errors are implemented with exceptions

#ifndef MAN_ERRORS_HPP
#define MAN_ERRORS_HPP

#include <string>
#include <sstream>

class Error
{
protected:
	std::string message;
	int line_no;

public:
	Error( const std::string& error_message, int line=0 ) : message(error_message), line_no(line) { }
	std::string what( )
	{
		if( line_no <= 0 )
		{
			std::string a = "Compiler Error: ";
			return a + message;
		}
		else
		{
			std::stringstream a;
			a << "Compiler Error Line " << line_no << ": " << message;
			return a.str( );
		}
	}
};



#endif



