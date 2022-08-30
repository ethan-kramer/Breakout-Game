// file: block.cpp
// lab: 11
// by: Ethan Kramer
// org: COP 2001, 202101, CRN: 10410
// desc: implementation of a block class object
// --------------------------------------------------------
#include "block.h"

// constructors
//---------------------------------------------------------
/**
default constructor - initialize empty block
*/
Block::Block() 
{
	xpos = 0;
	ypos = 0;
	width = 0;
	height = 0;
	color = fgcugl::White;
}


/**
properties constructor - initialize to parameter values
*/
Block::Block(int x, int y, int width, int height, fgcugl::Color color) 
{
	xpos = x;
	ypos = y;
	this->width = width;
	this->height = height;
	this->color = color;
}

// accessors
//---------------------------------------------------------
// getters	
int Block::getXCoordinate() { return xpos; }
int Block::getYCoordinate() { return ypos; }
int Block::getWidth() { return width; }
int Block::getHeight() { return height; }
fgcugl::Color Block::getColor() { return color; }

// setters
void Block::setXCoordinate(int value) { xpos = value; }
void Block::setYCoordinate(int value) { ypos = value; }
void Block::setWidth(int value) { width = value; }
void Block::setHeight(int value) { height = value; }
void Block::setColor(fgcugl::Color value) { color = value; }

// member methods
//---------------------------------------------------------
int Block::top() { return ypos + height; }
int Block::bottom() { return ypos; }
int Block::left() { return xpos; }
int Block::right() { return xpos + width; }

/**
return true if block is empty (width + height = 0)
*/
bool Block::isEmpty() { return width + height == 0; }

/**
draw a block object on OpenGL window
*/
void Block::draw()
{
	fgcugl::drawQuad(xpos, ypos, width, height, color);
}

/**
 draw a block with a border by drawing the block passed in on top
 of a background block

 Parameters:
 borderColor	color of the border of block
 borderSize		size of border in pixels
*/
void Block::drawWithBorder(fgcugl::Color borderColor, int borderSize)
{
	// draw background block
	Block background = Block(xpos, ypos, width, height, borderColor);
	background.draw();
	// draw inner block
	Block foreground = Block(xpos + borderSize, ypos + borderSize,
			width - 2 * borderSize, height - 2 * borderSize,
			color);
	foreground.draw();

}