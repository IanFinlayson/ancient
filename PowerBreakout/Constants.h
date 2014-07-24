// Constant Values	-	Copyright Ian Finlayson 2005
#ifndef CONSTANTS_H
#define CONSTANTS_H

//Window Stuff
const int WINDOW_WIDTH		=	640;
const int WINDOW_HEIGHT		=	480;
const int WINDOW_BORDER		=	32;

const int NUMBER_OF_LEVELS	=	5;

//Entity Stuff
const int BLOCK_WIDTH		=	32;
const int BLOCK_HEIGHT		=	16;

const int PADDLE_WIDTH		=	64;
const int PADDLE_HEIGHT		=	16;

const int BALL_SIZE			=	16;

//The higher the value, the slower the player can go
const int PLAYER_SPEED		=	3;

const int MAX_HORIZ_SPEED	=	500;

//Measured in milliseconds between frames
const int FRAME_RATE		=	30;

//Colors
const int NODRAW			=	0;
const int RED				=	1;
const int BLUE				=	2;
const int YELLOW			=	3;
const int ORANGE			=	4;
const int GREEN				=	5;
const int PURPLE			=	6;
const int WHITE				=	7;

const int BALL_COLOR		=	WHITE;
const int PADDLE_COLOR		=	WHITE;

//Level File stuff...( Note that changing these means changing the files )
//For now, we'll keep it even - by the way how do you spell colum?
const int BLOCKS_PER_ROW	=	( ( WINDOW_WIDTH - 2*WINDOW_BORDER ) / BLOCK_WIDTH );
const int BLOCKS_PER_COLUM	=	BLOCKS_PER_ROW;


#endif