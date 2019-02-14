#include "Scorecounter.h"


Scorecounter::Scorecounter(const Font & font_in, const Vei2 & displayPos_in, const Color color_in)
	:
	font(font_in),
	displayPos(displayPos_in),
	color(color_in)
{
}

void Scorecounter::add(const int points)
{
	score += points;
}

int Scorecounter::get() const
{
	return score;
}

void Scorecounter::display(Graphics & gfx) const
{
	font.printText(std::to_string(score), displayPos, gfx, color);
}
