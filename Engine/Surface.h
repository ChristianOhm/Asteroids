#pragma once
#include "Colors.h"
#include <string>
#include "RectI.h"

class Surface
{

private:
	int width;
	int height;
	Color* pColor;

public:
	Surface(int width_in, int height_in);
	Surface(std::string filename);
	Surface(const Surface& rhs);
	Surface& operator = (const Surface& rhs);

	~Surface();
	int getWidth() const;
	int getHeight() const;
	void putPixel(int x, int y, Color c);
	const Color getPixel(int x, int y) const;
	RectI getRect() const;



};