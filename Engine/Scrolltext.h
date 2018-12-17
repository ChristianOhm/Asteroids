#pragma once
#include <string>
#include <vector>
#include "Font.h"
#include <fstream>
#include "Graphics.h"
#include "RectI.h"




class Scrolltext
{
private:
	static constexpr int scrollSpeed = 15;
	const Font& font;
	const RectI window;
	const Color color;
	const int charsPerLine;
	std::vector<std::string> lines;
	std::vector<float> extraSpace;
	float topPos;
	

public:
	Scrolltext(const std::string filename_in, const Font& font_in, const Color color_in, const RectI window_in);
	void update(const float dt);
	void draw(Graphics& gfx) const;

};