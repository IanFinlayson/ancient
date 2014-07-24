// assem.cpp
// generates MIPS assembly from the
// intermediate code

#include <string>
#include <fstream>
#include <vector>
#include <cassert>
#include <algorithm>

#include "codegen.hpp"
#include "assem.hpp"


// writes the Mandrill std library
void writeStdLib( std::ofstream& out )
{
	std::ifstream std_lib( "stdlib.s" );
	std::string line;

	std::getline( std_lib, line );

	while( !std_lib.eof( ) )
	{
		out << line << "\n";
		std::getline( std_lib, line );
	}
}



// Returns a useable register for the argument
std::string getReg( Address arg, std::string arg_num, std::ofstream& out, bool real=false )
{

	if( arg.literal )
	{
		if( !real )
		{
			// load into a register
			out << "\tli $a" << arg_num << ", " << arg.lit << "\n";
			return "$a" + arg_num;
		}
		else
		{
			int regnum = atoi(arg_num.c_str( )) + 28;
			// load into a temp
			out << "\tli.s $f" << regnum << ", " << arg.lit << "\n";
			
			char temp[32];
			sprintf( temp, "$f%i", regnum );
			return std::string(temp);
		}
	}
	else	// not literal
	{
		if( !real )
		{
			if( arg.address->second.reg )
			{
				// simply return the register
				char temp[32];
				sprintf( temp, "$%i", arg.address->second.reg_num );
			
				return std::string(temp);
			}
			else
			{
				// load from memory
				out << "\tlw $a" << arg_num << ", " << arg.address->second.frame_pointer_offset << "($fp)\n";
				return "$a" + arg_num;
			}
		}
		else
		{
			if( arg.address->second.reg )
			{
				// simply return the register
				char temp[32];
				sprintf( temp, "$f%i", arg.address->second.float_rg_num );
				
				return std::string(temp);
			}
			else
			{
				// load from memory
				int regnum = atoi(arg_num.c_str( )) + 28;
				out << "\tl.s $f" << regnum << ", " << arg.address->second.frame_pointer_offset << "($fp)\n";
				
				char temp[32];
				sprintf( temp, "$f%i", regnum );
				return std::string(temp);
			}
		}

	}
}



// generates MIPS for an Intermediate Instruction
void genAssem( std::vector<Instruct>::iterator code, std::ofstream& out, SymbolTable& locals, SymbolTable& functions, const std::string& func_name )
{
	using namespace InterInstrs;
	switch( code->op )
	{
	case MOVE:
		{
			if( code->result.address->second.type.base != Type::REAL )
			{
				std::string arg = getReg( code->arg1, "0", out );
				if( code->result.address->second.reg )
				{
					out << "\tmove $" << code->result.address->second.reg_num << ", " << arg << "\n";
				}
				else
				{
					out << "\tsw " << arg << ", " << code->result.address->second.frame_pointer_offset << "($fp)\n";
				}
			}
			else
			{
				std::string arg = getReg( code->arg1, "0", out, true );
				if( code->result.address->second.reg )
				{
					out << "\tmov.s $f" << code->result.address->second.float_rg_num << ", " << arg << "\n";
				}
				else
				{
					out << "\ts.s " << arg << ", " << code->result.address->second.frame_pointer_offset << "($fp)\n";
				}
			}
			break;
		}

	case NOT:
		{
			std::string arg = getReg( code->arg1, "0", out );
			if( code->result.address->second.reg )
			{
				out << "\tnot $" << code->result.address->second.reg_num << ", " << arg << "\n";
			}
			else
			{
				out << "\tnot $a1, " << arg << "\n";
				out << "\tsw $a1, " << code->result.address->second.frame_pointer_offset << "($fp)\n";
			}
			break;
		}


	case ADD:
		{
			std::string arg1_reg = getReg( code->arg1, "0", out );
			std::string arg2_reg = getReg( code->arg2, "1", out );

			if( code->result.address->second.reg )
			{
				out << "\tadd $" << code->result.address->second.reg_num << ", " << arg1_reg << ", " << arg2_reg << "\n";
			}
			else
			{
				out << "\tadd $a0, " << arg1_reg << ", " << arg2_reg << "\n";
				out << "\tsw $a0, " << code->result.address->second.frame_pointer_offset << "($fp)\n";
			}
			break;
		}


	case ADD_S:
		{
			std::string arg1_reg = getReg( code->arg1, "0", out, true );
			std::string arg2_reg = getReg( code->arg2, "2", out, true );

			if( code->result.address->second.reg )
			{
				out << "\tadd.s $f" << code->result.address->second.float_rg_num << ", " << arg1_reg << ", " << arg2_reg << "\n";
			}
			else
			{
				out << "\tadd.s $f28, " << arg1_reg << ", " << arg2_reg << "\n";
				out << "\ts.s $f28, " << code->result.address->second.frame_pointer_offset << "($fp)\n";
			}
			break;
		}


	case MULT:
		{
			std::string arg1_reg = getReg( code->arg1, "0", out );
			std::string arg2_reg = getReg( code->arg2, "1", out );

			if( code->result.address->second.reg )
			{
				out << "\tmul $" << code->result.address->second.reg_num << ", " << arg1_reg << ", " << arg2_reg << "\n";
			}
			else
			{
				out << "\tmul $a0, " << arg1_reg << ", " << arg2_reg << "\n";
				out << "\tsw $a0, " << code->result.address->second.frame_pointer_offset << "($fp)\n";
			}

			break;
		}


	case MULT_S:
		{
			std::string arg1_reg = getReg( code->arg1, "0", out, true );
			std::string arg2_reg = getReg( code->arg2, "2", out, true );

			if( code->result.address->second.reg )
			{
				out << "\tmul.s $f" << code->result.address->second.float_rg_num << ", " << arg1_reg << ", " << arg2_reg << "\n";
			}
			else
			{
				out << "\tmul.s $f28, " << arg1_reg << ", " << arg2_reg << "\n";
				out << "\ts.s $f28, " << code->result.address->second.frame_pointer_offset << "($fp)\n";
			}

			break;
		}


	case SUB:
		{
			std::string arg1_reg = getReg( code->arg1, "0", out );
			std::string arg2_reg = getReg( code->arg2, "1", out );

			if( code->result.address->second.reg )
			{
				out << "\tsub $" << code->result.address->second.reg_num << ", " << arg1_reg << ", " << arg2_reg << "\n";
			}
			else
			{
				out << "\tsub $a0, " << arg1_reg << ", " << arg2_reg << "\n";
				out << "\tsw $a0, " << code->result.address->second.frame_pointer_offset << "($fp)\n";
			}
			break;
		}


	case SUB_S:
		{
			std::string arg1_reg = getReg( code->arg1, "0", out, true );
			std::string arg2_reg = getReg( code->arg2, "2", out, true );

			if( code->result.address->second.reg )
			{
				out << "\tsub.s $f" << code->result.address->second.float_rg_num << ", " << arg1_reg << ", " << arg2_reg << "\n";
			}
			else
			{
				out << "\tsub.s $f28, " << arg1_reg << ", " << arg2_reg << "\n";
				out << "\ts.s $f28, " << code->result.address->second.frame_pointer_offset << "($fp)\n";
			}
			break;
		}


	case DIV:
		{
			std::string arg1_reg = getReg( code->arg1, "0", out );
			std::string arg2_reg = getReg( code->arg2, "1", out );

			if( code->result.address->second.reg )
			{
				out << "\tdiv $" << code->result.address->second.reg_num << ", " << arg1_reg << ", " << arg2_reg << "\n";
			}
			else
			{
				out << "\tdiv $a0, " << arg1_reg << ", " << arg2_reg << "\n";
				out << "\tsw $a0, " << code->result.address->second.frame_pointer_offset << "($fp)\n";
			}
			break;
		}


	case DIV_S:
		{
			std::string arg1_reg = getReg( code->arg1, "0", out, true );
			std::string arg2_reg = getReg( code->arg2, "2", out, true );

			if( code->result.address->second.reg )
			{
				out << "\tdiv.s $f" << code->result.address->second.float_rg_num << ", " << arg1_reg << ", " << arg2_reg << "\n";
			}
			else
			{
				out << "\tdiv.s $f28, " << arg1_reg << ", " << arg2_reg << "\n";
				out << "\ts.s $f28, " << code->result.address->second.frame_pointer_offset << "($fp)\n";
			}
			break;
		}


	case REM:
		{
			std::string arg1_reg = getReg( code->arg1, "0", out );
			std::string arg2_reg = getReg( code->arg2, "1", out );

			if( code->result.address->second.reg )
			{
				out << "\trem $" << code->result.address->second.reg_num << ", " << arg1_reg << ", " << arg2_reg << "\n";
			}
			else
			{
				out << "\trem $a0, " << arg1_reg << ", " << arg2_reg << "\n";
				out << "\tsw $a0, " << code->result.address->second.frame_pointer_offset << "($fp)\n";
			}
			break;
		}


	case AND:
		{
			std::string arg1_reg = getReg( code->arg1, "0", out );
			std::string arg2_reg = getReg( code->arg2, "1", out );

			if( code->result.address->second.reg )
			{
				out << "\tand $" << code->result.address->second.reg_num << ", " << arg1_reg << ", " << arg2_reg << "\n";
			}
			else
			{
				out << "\tand $a0, " << arg1_reg << ", " << arg2_reg << "\n";
				out << "\tsw $a0, " << code->result.address->second.frame_pointer_offset << "($fp)\n";
			}
			break;
		}

	case OR:
		{
			std::string arg1_reg = getReg( code->arg1, "0", out );
			std::string arg2_reg = getReg( code->arg2, "1", out );

			if( code->result.address->second.reg )
			{
				out << "\tor $" << code->result.address->second.reg_num << ", " << arg1_reg << ", " << arg2_reg << "\n";
			}
			else
			{
				out << "\tor $a0, " << arg1_reg << ", " << arg2_reg << "\n";
				out << "\tsw $a0, " << code->result.address->second.frame_pointer_offset << "($fp)\n";
			}
			break;
		}


	case POW:
		{
			std::string arg1_reg = getReg( code->arg1, "0", out );
			std::string arg2_reg = getReg( code->arg2, "1", out );

			if( arg1_reg != "$a0" )
				out << "\tmove $a0, " << arg1_reg << "\n";

			if( arg2_reg != "$a1" )
				out << "\tmove $a1, " << arg2_reg << "\n";

			// call the pow function
			out << "\tjal powMAND\n";

			if( code->result.address->second.reg )
			{
				out << "\tmove $" << code->result.address->second.reg_num << ", $v0\n";
			}
			else
			{
				out << "\tsw $v0, " << code->result.address->second.frame_pointer_offset << "($fp)\n";
			}
			break;
		}

	case POW_S:
		// I have no earthly idea how this is done
		break;


	case LT:
		{
			std::string arg1_reg = getReg( code->arg1, "0", out );
			std::string arg2_reg = getReg( code->arg2, "1", out );

			if( code->result.address->second.reg )
			{
				out << "\tslt $" << code->result.address->second.reg_num << ", " << arg1_reg << ", " << arg2_reg << "\n";
			}
			else
			{
				out << "\tslt $a0, " << arg1_reg << ", " << arg2_reg << "\n";
				out << "\tsw $a0, " << code->result.address->second.frame_pointer_offset << "($fp)\n";
			}
			break;
		}


	case LT_S:
		{
			// result is type bool/int args are type real
			std::string arg1_reg = getReg( code->arg1, "0", out, true );
			std::string arg2_reg = getReg( code->arg2, "2", out, true );

			// this sets the coprocessor1 condition flag...
			out << "\tc.lt.s " << arg1_reg << ", " << arg2_reg << "\n";
			out << "\tjal finishRealComp\n";

			// put comp inro appropriate place
			if( code->result.address->second.reg )
			{
				out << "\tmove $" << code->result.address->second.reg_num << ", $v0\n";
			}
			else
			{
				out << "\tsw $v0, " << code->result.address->second.frame_pointer_offset << "($sp)\n";
			}

			break;
		}


	case LTE:
		{
			std::string arg1_reg = getReg( code->arg1, "0", out );
			std::string arg2_reg = getReg( code->arg2, "1", out );

			if( code->result.address->second.reg )
			{
				out << "\tsle $" << code->result.address->second.reg_num << ", " << arg1_reg << ", " << arg2_reg << "\n";
			}
			else
			{
				out << "\tsle $a0, " << arg1_reg << ", " << arg2_reg << "\n";
				out << "\tsw $a0, " << code->result.address->second.frame_pointer_offset << "($fp)\n";
			}
			break;
		}


	case LTE_S:
		{
			// result is type bool/int args are type real
			std::string arg1_reg = getReg( code->arg1, "0", out, true );
			std::string arg2_reg = getReg( code->arg2, "2", out, true );

			// this sets the coprocessor1 condition flag...
			out << "\tc.le.s " << arg1_reg << ", " << arg2_reg << "\n";
			out << "\tjal finishRealComp\n";

			// put comp inro appropriate place
			if( code->result.address->second.reg )
			{
				out << "\tmove $" << code->result.address->second.reg_num << ", $v0\n";
			}
			else
			{
				out << "\tsw $v0, " << code->result.address->second.frame_pointer_offset << "($sp)\n";
			}
			break;
		}


	case GT:
		{
			std::string arg1_reg = getReg( code->arg1, "0", out );
			std::string arg2_reg = getReg( code->arg2, "1", out );

			if( code->result.address->second.reg )
			{
				out << "\tsgt $" << code->result.address->second.reg_num << ", " << arg1_reg << ", " << arg2_reg << "\n";
			}
			else
			{
				out << "\tsgt $a0, " << arg1_reg << ", " << arg2_reg << "\n";
				out << "\tsw $a0, " << code->result.address->second.frame_pointer_offset << "($fp)\n";
			}
			break;
		}


	case GT_S:
		{
			// result is type bool/int args are type real
			std::string arg1_reg = getReg( code->arg1, "0", out, true );
			std::string arg2_reg = getReg( code->arg2, "2", out, true );

			// this sets the coprocessor1 condition flag...
			out << "\tc.lt.s " << arg2_reg << ", " << arg1_reg << "\n";
			out << "\tjal finishRealComp\n";

			// put comp inro appropriate place
			if( code->result.address->second.reg )
			{
				out << "\tmove $" << code->result.address->second.reg_num << ", $v0\n";
			}
			else
			{
				out << "\tsw $v0, " << code->result.address->second.frame_pointer_offset << "($sp)\n";
			}
			break;
		}


	case GTE:
		{
			std::string arg1_reg = getReg( code->arg1, "0", out );
			std::string arg2_reg = getReg( code->arg2, "1", out );

			if( code->result.address->second.reg )
			{
				out << "\tsge $" << code->result.address->second.reg_num << ", " << arg1_reg << ", " << arg2_reg << "\n";
			}
			else
			{
				out << "\tsge $a0, " << arg1_reg << ", " << arg2_reg << "\n";
				out << "\tsw $a0, " << code->result.address->second.frame_pointer_offset << "($fp)\n";
			}
			break;
		}


	case GTE_S:
		{
			// result is type bool/int args are type real
			std::string arg1_reg = getReg( code->arg1, "0", out, true );
			std::string arg2_reg = getReg( code->arg2, "2", out, true );

			// this sets the coprocessor1 condition flag...
			out << "\tc.le.s " << arg2_reg << ", " << arg1_reg << "\n";
			out << "\tjal finishRealComp\n";

			// put comp inro appropriate place
			if( code->result.address->second.reg )
			{
				out << "\tmove $" << code->result.address->second.reg_num << ", $v0\n";
			}
			else
			{
				out << "\tsw $v0, " << code->result.address->second.frame_pointer_offset << "($sp)\n";
			}
			break;
		}


	case EQ:
		{
			std::string arg1_reg = getReg( code->arg1, "0", out );
			std::string arg2_reg = getReg( code->arg2, "1", out );

			if( code->result.address->second.reg )
			{
				out << "\tseq $" << code->result.address->second.reg_num << ", " << arg1_reg << ", " << arg2_reg << "\n";
			}
			else
			{
				out << "\tseq $a0, " << arg1_reg << ", " << arg2_reg << "\n";
				out << "\tsw $a0, " << code->result.address->second.frame_pointer_offset << "($fp)\n";
			}
			break;
		}


	case EQ_S:
		{
			// result is type bool/int args are type real
			std::string arg1_reg = getReg( code->arg1, "0", out, true );
			std::string arg2_reg = getReg( code->arg2, "2", out, true );

			// this sets the coprocessor1 condition flag...
			out << "\tc.eq.s " << arg1_reg << ", " << arg2_reg << "\n";
			out << "\tjal finishRealComp\n";

			// put comp inro appropriate place
			if( code->result.address->second.reg )
			{
				out << "\tmove $" << code->result.address->second.reg_num << ", $v0\n";
			}
			else
			{
				out << "\tsw $v0, " << code->result.address->second.frame_pointer_offset << "($sp)\n";
			}
			break;
		}


	case NE:
		{
			std::string arg1_reg = getReg( code->arg1, "0", out );
			std::string arg2_reg = getReg( code->arg2, "1", out );

			if( code->result.address->second.reg )
			{
				out << "\tsne $" << code->result.address->second.reg_num << ", " << arg1_reg << ", " << arg2_reg << "\n";
			}
			else
			{
				out << "\tsne $a0, " << arg1_reg << ", " << arg2_reg << "\n";
				out << "\tsw $a0, " << code->result.address->second.frame_pointer_offset << "($fp)\n";
			}
			break;
		}


	case NE_S:
		{
			// result is type bool/int args are type real
			std::string arg1_reg = getReg( code->arg1, "0", out, true );
			std::string arg2_reg = getReg( code->arg2, "2", out, true );

			// this sets the coprocessor1 condition flag...
			out << "\tc.eq.s " << arg2_reg << ", " << arg1_reg << "\n";
			out << "\tjal finishRealComp\n";

			// invert the flag (now in $v0)
			out << "\tnot $v0, $v0\n";

			// put comp inro appropriate place
			if( code->result.address->second.reg )
			{
				out << "\tmove $" << code->result.address->second.reg_num << ", $v0\n";
			}
			else
			{
				out << "\tsw $v0, " << code->result.address->second.frame_pointer_offset << "($sp)\n";
			}
			break;
		}


	case LABEL:
		out << code->label << ":\n";
		break;


	case GOTO:
		out << "\tb " << code->label << "\n";
		break;


	case IF:
		{
			if( code->arg1.literal )	// this would be silly :) As a fututre optimization, I can compute the thingy now!
			{
				assert( code->arg1.lit_type != Type::REAL );
				out << "\tli $a0, " << code->arg1.lit << "\n";
				out << "\tbne $a0, $zero, " << code->label << "\n";
			}
			else // arg1 is a var
			{
				assert( code->arg1.address->second.type.base != Type::REAL );
				if( code->arg1.address->second.reg )
				{
                    out << "\tbne $zero, $" << code->arg1.address->second.reg_num << ", " << code->label << "\n";
				}
				else
				{
                    out << "\tlw $a0, " << code->arg1.address->second.frame_pointer_offset << "($fp)\n";
                    out << "\tbne $a0, $zero, " << code->label << "\n";
				}				
			}
			break;
		}


	case FUNCTION:
		{
			// First we print the function label:
			out << "\n\n" << code->label << ":\n";


			// Now we need to find the parameter count so we know how far into the stack to go
			unsigned int num_params = (unsigned int)functions.find( func_name )->second.type.parameters.size( );
			
			// Now we store our frame pointer. This is a pointer to the local variables - right now we just store it to $a3 (we must store fp later!)
			if( num_params > 0 ) out << "\taddi $a3, $sp, " << (num_params - 1)  * 4 << "\n";
			else out << "\tmove $a3, $sp\n";

			// Now we must setup our symbol table so that the parameters have their stack addresses
			int current_fp_o = 0;
			for( unsigned int i=0; i<functions.find( func_name )->second.type.parameters.size( ); i++ )
			{
				SymbolTable::iterator current_param = locals.find( functions.find( func_name )->second.type.param_names[i] );
				current_param->second.reg = false;
				current_param->second.frame_pointer_offset = -current_fp_o;
				current_fp_o += 4;
			}
			
			// Now we must allocate registers and stack space for our locals
			//save the old fp_o so we can increase stack correctly
			int curr_reg = 8;	// [8, 23]
			int locals_on_stack = 0;
			int curr_float_reg = 2;
			int floats_saved = 0;

			for( SymbolTable::iterator curr_local=locals.begin( ); curr_local!=locals.end( ); ++curr_local )
			{
				// If it's a parameter, don't do it!
				if( std::count( functions.find( func_name )->second.type.param_names.begin( ), functions.find( func_name )->second.type.param_names.end( ), curr_local->second.lexeme )
					!= 0 ) continue;

				if( curr_local->second.type.base != Type::REAL )
				{
					// Allocate space for this local (arg1)
					if( curr_reg <= 23 )
					{
						curr_local->second.reg = true;
						curr_local->second.reg_num = curr_reg;
						curr_reg++;
					}
					else
					{
						curr_local->second.reg = false;
						curr_local->second.frame_pointer_offset = -current_fp_o;
						current_fp_o += 4;
						locals_on_stack += 4;
					}
				}
				else
				{
					// Allocate space for this local (arg1)
					if( curr_float_reg <= 26 )
					{
						curr_local->second.reg = true;
						curr_local->second.float_rg_num = curr_float_reg;
						curr_float_reg += 2;
						floats_saved++;
					}
					else
					{
						curr_local->second.reg = false;
						curr_local->second.frame_pointer_offset = -current_fp_o;
						current_fp_o += 4;
						locals_on_stack += 4;
					}
				}
			}
			if( locals_on_stack ) out << "\taddi $sp, $sp, -" << locals_on_stack << "\n";	// add space for them (if needed)

			// store all floating regs that we use on stack
			if( floats_saved ) out << "\taddi $sp, $sp, -" << floats_saved*4 << "\n";
			for( int i=0; i<floats_saved; i++ )
			{
				out << "\ts.s $f" << (i + 1)*2 << ", " << i*4 << "($sp)\n";
			}

			// Now we must save registers we use to the stack...
			int regs_used = curr_reg - 8;
			out << "\taddi $sp, $sp -" << (regs_used * 4) + 4 << "\n";	// the +4 is for $ra
			int offset = 0;
			for( int i=0; i<regs_used; i++ )
			{
				out << "\tsw $" << i + 8 << ", " << offset << "($sp)\n";
				offset += 4;
			}
			// save $ra
			out << "\tsw $ra, " << offset << "($sp)\n";
			
			// save $fp and set the new one
			out << "\taddi $sp, $sp, -4\n";
			out << "\tsw $fp, 0($sp)\n";
			out << "\tmove $fp, $a3\n";



			// rather than have to perform clean up stuff for each return, we do it once and jump to this location on returns
			// jump around it
			out << "\tb " << func_name << "MANCONT\n";
			out << func_name << "MANDEND:\n";	// This should not be used as function name :/

			// pop off the $fp
			out << "\tlw $fp, 0($sp)\n";
			out << "\taddi $sp, $sp, 4\n";

			// pop off the return address
			out << "\tlw $ra, " << offset << "($sp)\n";
			
			// pop off the temporaries
			offset = 0;
			for( int i=0; i<regs_used; i++ )
			{
				out << "\tlw $" << i + 8 << ", " << offset << "($sp)\n";
				offset += 4;
			}
			// de-allocate stack space for saved registers
			out << "\taddi $sp, $sp, " << (regs_used * 4) + 4 << "\n";

			// pop off the float temps
			for( int i=0; i<floats_saved; i++ )
			{
				out << "\tl.s $f" << (i + 1)*2 << ", " << i*4 << "($sp)\n";
			}
			if( floats_saved ) out << "\taddi $sp, $sp, " << floats_saved*4 << "\n";
			
			// de-allocate space for locals
			if( locals_on_stack ) out << "\taddi $sp, $sp, " << locals_on_stack << "\n";
			// actually return now!
			out << "\tjr $ra\n\n";
			// label to skip over to
			out << func_name << "MANCONT:\n";

			break;	// done with FUNCTION!
		}

	case PARAM:
		// do nothing, already handeled
		break;


	case ARG:
		// do nothing, already handeled
		break;


	case CALL:
		{
			// We have to push the arguments on in order
			if( code->arg_count * 4 ) out << "\taddi $sp, $sp, -" << code->arg_count*4 << "\n";
			int offset = (code->arg_count - 1)*4;
			for(int back = code->arg_count; back>0; back-- )
			{
				Instruct arg = *(code - back);
			
				// push this argument

				if( arg.arg1.literal )
				{
					if( arg.arg1.lit_type != Type::REAL )
					{
						// put in reg a0
						out << "\tli $a0, " << arg.arg1.lit << "\n";
						out << "\tsw $a0, " << offset << "($sp)\n";
					}
					else
					{
						// put in reg f28
						out << "\tli.s $f28, " << arg.arg1.lit << "\n";
						out << "\ts.s $f28, " << offset << "($sp)\n";
					}
				}
				else
				{
					if( arg.arg1.address->second.type.base != Type::REAL )
					{
						if( arg.arg1.address->second.reg )
						{
							// move to reg a0
							out << "\tmove $a0, $" << arg.arg1.address->second.reg_num << "\n";
							out << "\tsw $a0, " << offset << "($sp)\n";
						}
						else
						{
							// load to a0
							out << "\tlw $a0, " << arg.arg1.address->second.frame_pointer_offset << "($fp)\n";
							out << "\tsw $a0, " << offset << "($sp)\n";
						}
					}
					else
					{
						if( arg.arg1.address->second.reg )
						{
							// move to reg f28
							out << "\tmov.s $f28, $f" << arg.arg1.address->second.float_rg_num << "\n";
							out << "\ts.s $f28, " << offset << "($sp)\n";
						}
						else
						{
							// load to f28
							out << "\tl.s $f28, " << arg.arg1.address->second.frame_pointer_offset << "($fp)\n";
							out << "\ts.s $f28, " << offset << "($sp)\n";
						}
					}

				}
			
				offset -= 4;
			}

			// jal
			out << "\tjal " << code->label << "\n";

			// pop off the arguments
			if( code->arg_count * 4 ) out << "\taddi $sp, $sp, " << code->arg_count*4 << "\n";

			break;
		}


	case LOCAL:
		// taken care of in FUNCTION
		break;


	case RETURN:
		{
			// put result into $v0/$f0

			if( code->arg1.literal )	// literal
			{
				if( code->arg1.lit_type != Type::REAL )
				{
					// li $v0, 5
					out << "\tli $v0, " << code->arg1.lit << "\n";
				}
				else
				{
					// li.s $f0, 5.023
					out << "\tli.s $f0, " << code->arg1.lit << "\n";
				}
			}
			else // not literal
			{
				if( code->arg1.address->second.type.base != Type::REAL )
				{
					if( code->arg1.address->second.reg )
					{
						// move $v0, $17
						out << "\tmove $v0, $" << code->arg1.address->second.reg_num << "\n";
					}
					else
					{
						// lw $v0, -12($fp)
						out << "\tlw $v0, " << code->arg1.address->second.frame_pointer_offset << "($fp)\n";
					}
				}
				else	// float
				{
					if( code->arg1.address->second.reg )
					{
						// mov.s $f0, $f12
						out << "\tmov.s $f0, $f" << code->arg1.address->second.float_rg_num << "\n";
					}
					else
					{
						// l.s $f0, -12($fp)
						out << "\tl.s $f0, " << code->arg1.address->second.frame_pointer_offset << "($fp)\n";
					}
				}
			}

			// branch to cleanup area
			out << "\tb " << func_name << "MANDEND\n";

			break;
		}

	case GETRET:
		// get $v0/$f0 and put into result
		assert( !code->result.literal );
		if( code->result.address->second.type.base != Type::REAL )
		{
			if( code->result.address->second.reg )
			{
				out << "\tmove $" << code->result.address->second.reg_num << ", $v0\n";
			}
			else
			{
				out << "\tsw $v0, " << code->result.address->second.frame_pointer_offset << "($fp)\n";
			}
		}
		else
		{
			if( code->result.address->second.reg )
			{
				out << "\tmov.s $f" << code->result.address->second.float_rg_num << ", $f0\n";
			}
			else
			{
				out << "\ts.s $f0, " << code->result.address->second.frame_pointer_offset << "($fp)\n";
			}
		}
		break;



	default:
		assert( false );
		break;
	}
}





// generates code from intermenidate code vector
void genAssem( std::vector<Instruct>& inter_code, const std::string& out_name, std::vector<SymbolTable>& locals, SymbolTable& functions )
{
	std::ofstream out( out_name.c_str( ) );
	out << "# File " << out_name << "\n# Compiled by Mandrill compiler\n\n";
	out << "\t.text\n\t.globl __start\n\n";
	out << "__start:\n\tjal main\n";	// jump to main
	
	// wait for input
	out << "\tla $a0, msg\n\tli $v0, 4\n\tsyscall\n";
	out << "\tli $v0, 5\n\tsyscall\n";

	out << "\t# quit\n\tli $v0, 10\n\tsyscall\n\n";	// end
	writeStdLib( out );

	// for each Instruction
	int current_function = -1;
	std::string func_name;
	for( std::vector<Instruct>::iterator i=inter_code.begin( ); i!=inter_code.end( ); ++i )
	{
		if( i->op == InterInstrs::FUNCTION )
		{
			current_function++;
			func_name = i->label;
		}

		genAssem( i, out, locals[current_function], functions, func_name );
	}

	out << "\n\n\t.data\nmsg:\t.asciiz \"\\nProgram Complete: Press Any Key To Continue\\n\"\n";
	out << "endl:\t.asciiz \"\\n\"\n";
	out << "hello:\t.asciiz \"Hello World!\"\n";
	out << "trueSTR:\t.asciiz \"true\"\n";
	out << "falseSTR:\t.asciiz \"false\"\n";
	out << "\n\n# End of file " << out_name << "\n# Have a nice day\n\n";
	out.close( );
}














