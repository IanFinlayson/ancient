/********************************************************************
*	SimException.h				 Ian Finlayson 2005					*
*																	*
*	This class is a general exeption class used by SimLib			*
*																	*
********************************************************************/

#include <cstring>

#ifndef SIM_EXCEPTION
#define SIM_EXCEPTION

namespace Sim
{
	class Exception
	{
	private:
		char* src;
		char* reas;

	public:
		Exception( const char* source, const char* reason )
		{
			src = strdup(source);
      reas = strdup(reason);
		}
		void WriteError( );
	};
}

#endif
