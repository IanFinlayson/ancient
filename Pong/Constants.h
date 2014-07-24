// Constants.h
// Ian Finlayson

// Defines all of the constants and global variables used...

#ifndef CONSTANTS_H
#define CONSTANTS_H

// These variables are constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const int		WALL_WIDTH		= 10;
const float		PI				= 3.141592653589793238462643383279502884197f;	// Probably more than float can hold
const int		SPEED_FACTOR	= 3;	// Controls how much where you hit the ball affects its velocity

const float		BALL_START_X		= 100.0f;				// Starting positions
const float		BALL_START_Y		= WINDOW_HEIGHT / 2.0f;

const int		PADDLE_WIDTH		= 15;
const float		PADDLE_X			= WINDOW_WIDTH - PADDLE_WIDTH - 10.0f;
const float		PADDLE_Y_START		= BALL_START_Y;


// These variables are global across all files (they are defined in main.cpp):
// They can take on different values based on the user's difficulty setting
// (Once the game starts, they are used as constants...)
extern int		BALL_RADIUS;
extern float	BALL_SPEED;

extern int		PADDLE_HEIGHT;
extern float	PADDLE_SPEED;


// These variables are the values which may be applied to the variables above...
const int		BALL_RADIUS_EASY			= 15;
const int		BALL_RADIUS_MEDIUM			= 10;
const int		BALL_RADIUS_HARD			= 5;

const float		BALL_SPEED_EASY				= 200.0;	// Pixels per second
const float		BALL_SPEED_MEDIUM			= 300.0;	// Pixels per second
const float		BALL_SPEED_HARD				= 400.0;	// Pixels per second

const int		PADDLE_HEIGHT_EASY			= 100;
const int		PADDLE_HEIGHT_MEDIUM		= 75;
const int		PADDLE_HEIGHT_HARD			= 50;

const float		PADDLE_SPEED_EASY			= 250.0;	// Pixels per second
const float		PADDLE_SPEED_MEDIUM			= 300.0;	// Pixels per second
const float		PADDLE_SPEED_HARD			= 450.0;	// Pixels per second

#endif