// SnowStorm.h
// Ian Finlayson

// Implements snowy particles...

#ifndef SNOW_STORM
#define SNOW_STORM

#include "Vector.h"

class SnowStorm
{
private:
	static const int MAX_FLAKES = 1000;

	// A flake has position and velocity
	struct Flake
	{
		Vector<float, 2> position;
		Vector<float, 2> velocity;
	};

	// We keep them in an array
	Flake flakes[MAX_FLAKES];

public:
	SnowStorm( );
	
	void Draw( );
	void Update( float elapsed_time );
};

#endif