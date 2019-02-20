#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "Font.h"
#include "Graphics.h"
#include "Colors.h"


class Highscores 
{
private:
	std::string filename = "Data/Highscore.dat";
	const Font& font;
	const Color color = Colors::Green;
	const Color highlight = Colors::MakeRGB(225u, 255u, 180u);
	static constexpr int maxCharName = 15;
	class Entry
	{
	public:
		std::string name;
		int stage;
		int score;
		Entry(std::string name_in, int stage_in, int score_in)
			:
			name(name_in),
			stage(stage_in),
			score(score_in)
		{
		}
		Entry() = default;
		bool operator< (int rhs_score) const
		{
			return (score < rhs_score);
		}
	};
	std::vector<Entry> entries;
	std::vector<Entry>::iterator newEntry = entries.end();

public:
	Highscores(const Font& font_in);
	bool checkNew(const int currentScore) const;
	void add (int currentStage, int currentScore);
	void save();
	void display(Graphics& gfx);
	bool addNametoNew(char char_in);

};