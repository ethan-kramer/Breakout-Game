// file: breakout.cpp
// lab: 11
// by: Ethan Kramer
// org: COP 2001, 202101, CRN: 10410
// desc: main configuration header for 2D Breakout game
// --------------------------------------------------------
#ifndef BREAKOUT_H
#define BREAKOUT_H

#include <string>	// for window title

#include "fgcugl.h"	// OpenGL Library
#include "block.h"	// block object

//------------------------------------------------------------
// enums

// flag values
enum Direction {
	DIR_EXIT = -1,
	DIR_NONE,
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT
};



//-----------------------------------------------------------
// global constants

// main game window properties
const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 604;
const std::string WINDOW_TITLE = "Breakout";

// game settings
const double FRAME_RATE = 1.0 / 60.0;


// game components


// player paddle properties
int PADDLE_WIDTH = 96;
int PADDLE_HEIGHT = 20;

// ball properties
const int BALL_SIZE = 8;	// radius of ball in pixels
const fgcugl::Color BALL_COLOR = fgcugl::Blue;
const float BALL_SPEED_X = 40.0;	// ball speed horizontally in pixels
const float BALL_SPEED_Y = 70.0;	// ball speed vertically in pixels


// boarder walls
const int WALL_SIZE = 2;

// bricks
const int BRICK_WIDTH = 64;
const int BRICK_HEIGHT = 20;
const int BRICKS_COLUMNS = 14;				// number of bricks across
const int BRICKS_ROWS = 8;					// number of bricks down
const int BRICKS_START_X = WALL_SIZE;
const int BRICKS_START_Y = (WINDOW_HEIGHT - 2 * WALL_SIZE) / 2;

//-----------------------------------------------------------
// structures

// border walls
struct Walls {
	Block top;
	Block bottom;
	Block left;
	Block right;
};


#endif // BREAKOUT_H