// file: paddle.h
// lab: 11
// by: Ethan Kramer
// org: COP 2001, 202101, CRN: 10410
// desc: declaration of a player paddle block class object
// --------------------------------------------------------
#ifndef PADDLE_H
#define PADDLE_H

#include "fgcugl.h"
#include "block.h"

class Paddle {
public:
	// global defines
	enum PaddleDirection {
		None,
		Left,
		Right
	};

	// constructors
	Paddle();
	Paddle(int x, int y, int width, int height, fgcugl::Color color = fgcugl::Green);

	// accessors
	// getters	
	Block getPaddle();			// READ ONLY 
	float getXCoordinate();
	float getYCoordinate();
	int getWidth();
	int getHeight();
	fgcugl::Color getColor();
	float getVelocity();

	// setters
	void setXCoordinate(float value);
	void setYCoordinate(float value);
	void setWidth(int value);
	void setHeight(int value);
	void setColor(fgcugl::Color value);
	void setVelocity(float value);

	// member methods
	int top();
	int bottom();
	int left();
	int right();

	bool isEmpty();

	PaddleDirection currentDirection();

	void update(PaddleDirection next, Block leftWall, Block rightWall, float lag);
	void draw(float lag);
	void drawWithBorder(float lag,	fgcugl::Color borderColor = fgcugl::Black, int borderSize = 1);

private:
	Block paddle;
	float currentX;
	float currentY;
	float velocity;
	
};

#endif //PADDLE_H