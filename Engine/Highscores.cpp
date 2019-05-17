#include "Highscores.h"


void Highscores::add(int currentStage, int currentScore)
{
	auto i = entries.begin();

	for (auto e = entries.end(); i != e; ++i)
	{
		if (i->score < currentScore)
		{
			break;
		}
	}
	newEntry = entries.insert(i, Entry("", currentStage, currentScore));
	newHighscoreEntered = true;
	if (entries.size() > 10)
	{
		entries.resize(10);
	}

	
}

void Highscores::save()
{
	std::ofstream file (filename, std::ios::binary);
	for (auto& e : entries)
	{
		file << e.name << '\n';
		file.write(reinterpret_cast<char*> (&e.stage), sizeof(int));
		file.write(reinterpret_cast<char*> (&e.score), sizeof(int));

	}
}

void Highscores::display(Graphics & gfx)
{
	Vei2 pos (132, 120);
	font.printText("HIGHSCORES", Vei2(335, 50), gfx, color);
	font.printText("NAME", pos, gfx, color);
	font.printText("STAGE", pos + Vei2(308, 0), gfx, color);
	font.printText("SCORE", pos + Vei2(424, 0), gfx, color);
	pos += Vei2(0, 60);
	for (std::vector<Entry>::iterator s = entries.begin(), e = entries.end(); s != e; ++s )
	{
		if (newHighscoreEntered && s == newEntry)
		{
			font.printText(s->name, pos, gfx, highlight);
			font.printText(std::to_string(s->stage), pos + Vei2(340, 0), gfx, highlight);
			font.printText(std::to_string(s->score), pos + Vei2(440, 0), gfx, highlight);
			pos += Vei2(0, 40);
		}
		else
		{
			font.printText(s->name, pos, gfx, color);
			font.printText(std::to_string(s->stage), pos + Vei2(340, 0), gfx, color);
			font.printText(std::to_string(s->score), pos + Vei2(440, 0), gfx, color);
			pos += Vei2(0, 40);
		}
		
	}
}

bool Highscores::addNametoNew(char char_in)
{
	if (char_in == 13 && newEntry->name.length() > 0)
	{
		newEntry = entries.end();
		return true;
	}

	if (char_in == 8 && newEntry->name.length() > 0)
	{
		newEntry->name.pop_back();
		return false;
	}

	if (char_in >= 32 && char_in <= 126)
	{
		newEntry->name += char_in;
		if (newEntry->name.length() >= maxCharName)
		{
			newEntry = entries.end();
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

Highscores::Highscores(const Font& font_in)
	:
	font(font_in)
{
	std::ifstream file(filename, std::ios::binary);
	Entry temp;
	file.seekg(0, std::ios::end);
	int end = file.tellg();
	file.seekg(0);
	while (file.tellg() < end)
	{
		std::getline(file, temp.name);
		file.read(reinterpret_cast<char*> (&temp.stage), sizeof(int));
		file.read(reinterpret_cast<char*> (&temp.score), sizeof(int));
		entries.push_back(temp);

	}
}

bool Highscores::checkNew(const int currentScore) const
{
	return ((currentScore > 0 && entries.size() < 10 || (entries.size() > 0 && entries.back() < currentScore)));
}
