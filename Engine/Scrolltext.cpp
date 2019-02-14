 #include "Scrolltext.h"
#include <algorithm>

Scrolltext::Scrolltext(const std::string filename_in, const Font & font_in, const Color color_in, const RectI window_in)
	:
	font(font_in),
	color (color_in),
	window(window_in),
	charsPerLine(window.getWidth() / font.charWidth)
	
{
	topPos = (float)window.bottom;
	std::ifstream textFile(filename_in);
	std::string lineFromFile;
	while (std::getline(textFile, lineFromFile))
	{
		lineFromFile += '\n';
		while (true)
		{
			if (lineFromFile.length() <= charsPerLine)
			{
				lines.push_back(lineFromFile);
				break;
			}
			else
			{
				int pos = (int)lineFromFile.rfind(' ', charsPerLine);
				if (!pos)
				{
					pos = charsPerLine;
				}
				lines.push_back(lineFromFile.substr(0, pos));
				lineFromFile.erase(0, pos+1);
			}

		}
	}

	for (const auto& l : lines)
	{
		int nSpace = (int)std::count(l.begin(), l.end(), ' ');
		if (l.back() != '\n' && nSpace != 0)
		{
			
			extraSpace.push_back((charsPerLine - l.size())*font.charWidth / (float)nSpace);
		}
		else
		{
			extraSpace.push_back(0.0f);
		}
	}
}

void Scrolltext::update(const float dt)
{
	topPos -= scrollSpeed * dt;
}

void Scrolltext::draw(Graphics & gfx) const
{
	int lowerLine;
	int upperLine;
	if (topPos >= window.top)
	{
		lowerLine = 0;
		upperLine = std::min(((window.bottom - (int)topPos) / font.charHeight), (int)lines.size()-1);
	}
	else
	{
		lowerLine = std::max(((window.top - (int)topPos) / font.charHeight ), 0);
		upperLine = std::min((lowerLine + window.getHeight() / font.charHeight), (int)lines.size()-1);
	}


	for (int counter = lowerLine; counter <= upperLine; ++counter)
	{
		font.printText(lines[counter], Vei2(window.left, (int)topPos + counter * font.charHeight), gfx, color, window, extraSpace[counter] );
	}
}
