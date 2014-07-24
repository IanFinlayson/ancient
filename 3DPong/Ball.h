// Ball.h
// Ian Finlayson

#ifndef BALL_H
#define BALL_H

#include <windows.h>
#include <GL/glu.h>

#include "Vector.h"

class Paddle;	// Forward declaration of paddle class

class Ball
{
private:
	Vector<float, 3> position;
	Vector<float, 3> velocity;

	// The ball is drawn with a glu quadric sphere
	GLUquadric* sphere;

	Paddle* paddle;

public:
	Ball( Paddle* paddle );
	~Ball( );

	void Reset( );
	bool Update( float elapsed_time );
	void Draw( );
};
#endif