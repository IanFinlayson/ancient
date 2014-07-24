/********************************************************************
*	SimException.cpp			 Ian Finlayson 2005					*
*																	*
*	Implements SimException											*
*																	*
********************************************************************/

#include "SimException.h"
#include <fstream>

void Sim::Exception::WriteError( )
{			
	std::fstream errorlog;

	errorlog.open( "Error.txt", std::ios::out | std::ios::app );

	errorlog << "Error Ocurred in............" << src << std::endl
		<< "Error Happened because......" << reas << std::endl << std::endl;

	errorlog.close( );
}