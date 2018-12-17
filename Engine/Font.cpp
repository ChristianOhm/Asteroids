#include "Font.h"
#include <assert.h>


RectI Font::getCharRect(const char c) const
{
	assert(c > firstChar && c <= lastChar);
	int nChar = c - firstChar;
	int row = nChar / nColumns;
	int column = nChar % nColumns;
	return RectI(Vei2(column * charWidth, row * charHeight), 
		charWidth, charHeight);



}

void Font::printText(const std::string & text_in, const Vei2 & pos, Graphics & gfx, const Color & color) const
{
	printText(text_in, pos, gfx, color, gfx.getScreenRect());
}

void Font::printText(const std::string & text_in, const Vei2 & pos, Graphics & gfx, const Color & color, const RectI & clippingRect) const
{
	printText(text_in, pos, gfx, color, clippingRect, 0);
}

void Font::printText(const std::string & text_in, const Vei2 & pos, Graphics & gfx, const Color & color, const RectI & clippingRect, float extraSpace) const
{
	float x_text = (float)pos.x;
	int y_text = pos.y;

	for (const char& c : text_in)
	{
		if (c == '\n')
		{
			y_text += charHeight;
			x_text = (float)pos.x;
			continue;
		}
		if (c == ' ')
		{
			x_text += (charWidth + extraSpace);
			continue;
		}

		if (c > firstChar && c <= lastChar)
		{
			gfx.drawSprite(fontsheet, color, getCharRect(c), clippingRect, (int)x_text, y_text, chroma);
		}
		x_text += charWidth;
	}
}

Font::Font(const std::string & filename, const Color & chroma_in)
	:

	fontsheet(filename),
	chroma (chroma_in)
{
	charHeight = fontsheet.getHeight() / nRows;
	charWidth = fontsheet.getWidth() / nColumns;
}



