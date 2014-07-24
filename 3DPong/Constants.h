// Constants.h
// Ian Finlayson

#ifndef CONSTANTS_H
#define CONSTANTS_H

// These variables are contants...
const float FRONT_Z = -750.0f;		// Z value of the front wall
const float BACK_Z = -3050.0f;		// Z value of the back wall
const float WINDOW_WIDTH = 800;
const float WINDOW_HEIGHT = 600;

// Difficulty stuff...
const float BALL_RADIUS_SIMPLE = 200.0f;
const float BALL_RADIUS_EASY = 100.0f;
const float BALL_RADIUS_MEDIUM = 50.0f;
const float BALL_RADIUS_HARD = 25.0f;
const float BALL_RADIUS_INSANE = 10.0f;

const float BALL_SPEED_SIMPLE = 200.0f;
const float BALL_SPEED_EASY = 500.0f;
const float BALL_SPEED_MEDIUM = 750.0f;
const float BALL_SPEED_HARD = 1000.0f;
const float BALL_SPEED_INSANE = 1600.0f;

const float PADDLE_SIZE_SIMPLE = 400.0f;
const float PADDLE_SIZE_EASY = 200.0f;
const float PADDLE_SIZE_MEDIUM = 150.0f;
const float PADDLE_SIZE_HARD = 100.0f;
const float PADDLE_SIZE_INSANE = 50.0f;

const float PADDLE_SPEED_SIMPLE = 2000.0f;
const float PADDLE_SPEED_EASY = 1000.0f;
const float PADDLE_SPEED_MEDIUM = 500.0f;
const float PADDLE_SPEED_HARD = 500.0f;
const float PADDLE_SPEED_INSANE = 1000.0f;


// These variables are global...
extern bool FULL_SCREEN;
extern bool SOUND_ON;

extern float BALL_RADIUS;
extern float BALL_SPEED;

extern float PADDLE_WIDTH;
extern float PADDLE_HEIGHT;
extern float PADDLE_SPEED;

extern int bounces;
extern int losses;

#endif