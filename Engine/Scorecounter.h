#pragma once
#include "Graphics.h"
#include "Font.h"
#include "Vei2.h"

class Scorecounter
{
private:
	int score = 0;
	const Font& font;
	Vei2 displayPos;
	Color color;


public:
	Scorecounter(const Font& font_in, const Vei2& displayPos_in, const Color color_in);
	void add(const int points);
	int get() const;
	void display(Graphics& gfx) const;

};