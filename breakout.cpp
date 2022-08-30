// file: breakout.cpp
// lab: 11
// by: Ethan Kramer
// org: COP 2001, 202101, CRN: 10410
// desc: main application file for 2D Breakout game
// --------------------------------------------------------
#include <iostream>		// console library for debugging
#include "fgcugl.h"		// fgcu OpenGL library
#include "breakout.h"	// main application header file
#include "paddle.h"		// player paddle
#include "ball.h"		

// function prototypes
// creation
void createBricks(Block bricks[BRICKS_ROWS][BRICKS_COLUMNS]);		// arrays always passed by reference (C++)

// main game loop processing
Direction processInput();
bool update(Ball& ball, Paddle& paddle, Block bricks[BRICKS_ROWS][BRICKS_COLUMNS], Walls walls, double lag, Direction next);
void render(Ball ball, Paddle paddle, Block bricks[BRICKS_ROWS][BRICKS_COLUMNS], Walls walls, double lag);


int main()
{	
	// main game window
	fgcugl::openWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, false);

	// border walls
	Walls walls;
	walls.top = Block(0, WINDOW_HEIGHT - WALL_SIZE, WINDOW_WIDTH, WALL_SIZE);
	walls.bottom = Block(0, 0, WINDOW_WIDTH, WALL_SIZE);
	walls.left = Block(0, 0, WALL_SIZE, WINDOW_HEIGHT);
	walls.right = Block(WINDOW_WIDTH - WALL_SIZE, 0, WALL_SIZE, WINDOW_HEIGHT);
	

	// create player paddle
	int padx = WINDOW_WIDTH / 2 - PADDLE_WIDTH / 2;
	int pady = walls.bottom.top() + 1 + PADDLE_HEIGHT;
	Paddle paddle = Paddle(padx, pady, PADDLE_WIDTH, PADDLE_HEIGHT, fgcugl::Green);

	// create the ball
	float ballX = WINDOW_WIDTH / 2.0;
	float ballY = paddle.top() + BALL_SIZE + 1;
	Ball ball = Ball(ballX, ballY, BALL_SPEED_X, BALL_SPEED_Y, BALL_SIZE, BALL_COLOR);


	// create the rack of bricks
	Block bricks[BRICKS_ROWS][BRICKS_COLUMNS];
	createBricks(bricks);

	// directions
	Direction currentDirection = DIR_NONE, nextDirection = DIR_NONE;
	
	// timing for game loop
	double startTime = fgcugl::getTime();
	double finishTime = 0.0, deltaTime = 0.0;

	// main game loop
	bool gameover = false;
	while (!gameover)
	{
		finishTime = fgcugl::getTime();			// stop frame timer 
		deltaTime += finishTime - startTime;	// add current lag
		startTime = finishTime;

		//processInput
		nextDirection = processInput();

		// update
		while (deltaTime >= FRAME_RATE)
		{
			gameover = update(ball, paddle, bricks, walls, deltaTime, nextDirection);
			deltaTime -= FRAME_RATE;
		}

		// render
		render(ball, paddle, bricks, walls, deltaTime);

		//get keyboard and window move/close events
		fgcugl::getEvents();

		// end game if window closed
		gameover = gameover || fgcugl::windowClosing();
	}

	fgcugl::cleanup();

	return 0;
} // end main


/**
 create the rack of bricks 

 Parameters:
 bricks				the array of blocks

 Return:
 void
*/
void createBricks(Block bricks[BRICKS_ROWS][BRICKS_COLUMNS])
{
	int y = BRICKS_START_Y;

	for (int row = 0; row < BRICKS_ROWS; row++)
	{
		int x = BRICKS_START_X;

		for (int column = 0; column < BRICKS_COLUMNS; column++)
		{
			fgcugl::Color blockColor;

			// assign color by row number
			switch (row)
			{
			case 0:
			case 1:
				blockColor = fgcugl::Yellow;
				break;
			case 2:
			case 3:
				blockColor = fgcugl::Green;
				break;
			case 4:
			case 5:
				blockColor = fgcugl::Orange;
				break;
			case 6:
			case 7:
				blockColor = fgcugl::Red;
			}	// block color
			
			bricks[row][column] = Block(x, y, BRICK_WIDTH, BRICK_HEIGHT, blockColor);

			x += BRICK_WIDTH;

		}	// columns

		y += BRICK_HEIGHT;
	}	// rows
}	// end createBricks

/**
 get user keyboard input

 Parameters:

 Return:
 Direction		next direction to move (default is DIR_NONE)
*/
Direction processInput()
{
	Direction direction = DIR_NONE;

	// get keyboard input
	char key = fgcugl::getKey();

	switch (key)
	{
	case 'W':	// up
		direction = DIR_UP;
		break;
	case 'S':	// down
		direction = DIR_DOWN;
		break;
	case 'A':	// left
		direction = DIR_LEFT;
		break;
	case 'D':	// right
		direction = DIR_RIGHT;
		break;
	case 'X':
		direction = DIR_EXIT;
	}

	return direction;
}	// end processInput


/**
 update game objects
 Parameters:
 ball			location and speed of ball
 paddle			player paddle
 walls			border walls for collision checks 
 lag			current frame time plus lag
 bricks			the rack of blocks
 next			next direction to move the ball

 Returns:
 bool			true if game should end
*/
bool update(Ball& ball, Paddle& paddle, Block bricks[BRICKS_ROWS][BRICKS_COLUMNS], Walls walls, double lag, Direction next)
{
	bool quit = false;
	

		Paddle::PaddleDirection paddleDir = Paddle::None;
		if (next == DIR_LEFT)
		{
			paddleDir = Paddle::Left;
		}
		else if (next == DIR_RIGHT)
		{
			paddleDir = Paddle::Right;
		}

		paddle.update(paddleDir, walls.left, walls.right, lag);


	// adjust ball to new position based on speed
		ball.update(lag);

	// see if ball hit bottom wall
	if (ball.collisionCheck(walls.bottom))
	{
		quit = true;
	}
	else
	{
		// see if ball has hit a wall
		int collision = ball.collisionCheck(walls.top);
		collision |= ball.collisionCheck(walls.bottom);
		collision |= ball.collisionCheck(walls.left);
		collision |= ball.collisionCheck(walls.right);

		// check for collisions of ball with the bricks
		// don't start checking until the ball has reached
		// the lowest brick
		if (ball.top() >= BRICKS_START_Y)
		{
			for (int row = 0; row < BRICKS_ROWS; row++)
			{
				for (int column = 0; column < BRICKS_COLUMNS; column++)
				{
					// if current block not broken (has a y coordinate)
					if (!bricks[row][column].isEmpty())
					{
						Ball::CollisionType brickCollision = ball.collisionCheck(bricks[row][column]);
						if (brickCollision)
						{
							collision |= brickCollision;
							bricks[row][column] = Block();
						}
					}	// not broken
				}	// column
			}	// row

		}	// ball reached rack 
		else
		{
			// check for collision with player paddle
			collision |= ball.collisionCheck(paddle.getPaddle());
		}

		ball.doCollision(collision);
	} // collsion checking
		
	return quit;
}	// end update

/**
 draw game objects on the OpenGL window

 Parameters:
 ball			location and speed of ball
 paddle			location of player paddle	
 walls			border walls for collision checks
 lag			build up of lag between frames


 Returns:
 void
*/
void render(Ball ball, Paddle paddle, Block bricks[BRICKS_ROWS][BRICKS_COLUMNS], Walls walls, double lag)
{
	// draw player paddle
	paddle.draw(lag);

	// draw ball in new position relative to current lag
	ball.draw(lag);
	// draw bricks
	for (int row = 0; row < BRICKS_ROWS; row++)
	{
		for (int column = 0; column < BRICKS_COLUMNS; column++)
		{
			// if current block not broken (has a y-coordinate)
			if (!bricks[row][column].isEmpty())
			{
				bricks[row][column].drawWithBorder();
			}
		}	// column 
	}	// row

	

	// draw walls
	walls.top.draw();
	walls.bottom.draw();
	walls.left.draw();
	walls.right.draw();

	// paint new scene to window
	fgcugl::windowPaint();
}	// end render