// file: ball.cpp
// lab: 11
// by: Ethan Kramer
// org: COP 2001, 202101, CRN: 10410
// desc: implementation of a ball class object
// --------------------------------------------------------
#define _USE_MATH_DEFINES	// enable M_PI
#include <math.h>		// for sqrt, pwr, atan2, M_PI
#include "ball.h"
#include "block.h"			// quad block object


// constructors
// --------------------------------------------------------
/**
property constructor - initial ball location
*/
Ball::Ball(float x, float y, float velocityX, float velocityY, int size, fgcugl::Color color)
{
	xpos = x;
	ypos = y;
	this->velocityX = velocityX;
	this->velocityY = velocityY;
	radius = size;
	this->color = color;

}

// accessors
// --------------------------------------------------------
// getters
float Ball::getXCoordinate() { return xpos; }
float Ball::getYCoordinate() { return ypos; }
float Ball::getXVelocity() { return velocityX; }
float Ball::getYVelocity() { return velocityY; }
int Ball::getSize() { return radius; }
fgcugl::Color Ball::getColor() { return color; }


// setters
void Ball::setXCoordinate(float value) { xpos = value; }
void Ball::setYCoordinate(float value) { ypos = value; }
void Ball::setXVelocity(float value) { velocityX = value; }
void Ball::setYVelocity(float value) { velocityY = value; }
void Ball::setSize(int value) { radius = value; }
void Ball::setColor(fgcugl::Color value) { color = value; }

// member methods
// --------------------------------------------------------
int Ball::top() { return ypos + radius; }
int Ball::bottom() { return ypos - radius; }
int Ball::left() { return xpos - radius; }
int Ball::right() { return xpos + radius; }

/**
update location of ball each frame relative to lag 

Parameters:
lag				current frame lag
*/
void Ball::update(float lag)
{
	// adjust ball to new position based on speed
	xpos += velocityX * lag;
	ypos += velocityY * lag;
}

/**
draw a ball object on OpenGL window relative to the current x/y with lag
*/
void Ball::draw(float lag)
{
	// draw ball in new position relative to current lag
	float currX = xpos + velocityX * lag;
	float currY = ypos + velocityY * lag;
	fgcugl::drawCircle(currX, currY, radius, color);

}

/**
 check for collisions between a circle and rectangle

 Parameters:
 block		the locationa and size of block to check

 Returns:
 CollisionType	collision in the veritcal, horizontal, or none
*/

Ball::CollisionType Ball::collisionCheck(Block block)
{

	CollisionType quadrant = None;

	// coordinates on the rectangle closest  to the circle center
	float testX = xpos;	// circle X between rectangle X and rectangle X + width
	float testY = ypos;	// circle Y between rectangle Y and rectangle Y + height

	// find the horizontal coordinate on the rectangle closest to circle center
	if (xpos < block.left())	//circle left of rectangle
	{
		testX = block.left();
	}
	else if (xpos > block.right())	// circle right of rectangle
	{
		testX = block.right();
	}

	// find the vertical coordinate on the rectangle closest to circle center
	if (ypos < block.bottom())	//circle below rectangle
	{
		testY = block.bottom();
	}
	else if (ypos > block.top())	// circle above rectangle
	{
		testY = block.top();
	}

	// calc difference between circle and rectangle (x,y) coordinates
	float diffX = testX - xpos;
	float diffY = testY - ypos;

	// calc distance from circle center to rectangle (Pythagorean Theorem)
	float distance = sqrt(pow(diffX, 2) + pow(diffY, 2));

	// if circle is closer to rectangle than its radius
	// then we had a collision
	if (distance < radius)
	{

		float radians = atan2(diffY, diffX);
		float angle = radians * 180.0 / M_PI;	// relative to the x-axis
		float degrees = 90.0 - angle;		// rotate angle to the left 90 degrees
		float cardinal = (degrees > 0.0 ? degrees : degrees + 360.0);	// fix negative cardinal degrees

		// we are dividing the ball into 45 degree quadrants with the upper and lower
		// resulting in a vertical collision, and the left and right resulting a horizontal collision
		if ((cardinal >= 315 || cardinal < 45) ||  	// top quadrant
			(cardinal >= 135 && cardinal <= 225))	// bottom quadrant
		{
			quadrant = Vertical;
		}
		else										// left or right quadrant
		{
			quadrant = Horizontal;
		}	// quadrants

	}	// end collision

	return quadrant;

} // collision check

/**
adjust trajectory of ball for collisions

Parameters:
collisions		combo of horiz/vert/none
*/
void Ball::doCollision(int collisions)
{
	// if not None
	if (collisions)
	{
		// for a collision we invert velocities and then move the ball its radius value
// in the new direction to get it out of the object it is colliding with, so we don't
// end up w/ a 2nd collision on the next update

		if (collisions & Vertical)
		{
			velocityY *= -1;	// invert veritcal velocity
			ypos += (velocityY > 0.0 ? 1 : -1) * (radius + 1);	// move ball out of object by radius +1
		}	// vertical collision

		if (collisions & Horizontal)
		{
			velocityX *= -1;	// invert horizontal velocity
			xpos += (velocityX > 0.0 ? 1 : -1) * (radius + 1);	// move ball out of object by radius +1
		}	// horizontal collision



	}
}