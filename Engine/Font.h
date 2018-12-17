#pragma once
#include "Surface.h"
#include <vector>
#include <string>
#include "Vei2.h"
#include "Graphics.h"
#include "RectI.h"
#include "Colors.h"


class Font
{
private:
	Surface fontsheet;
	RectI getCharRect(const char c) const;
	static constexpr int nColumns = 32;
	static constexpr int nRows = 3;
	static constexpr char firstChar = ' ';
	static constexpr char lastChar = '~';
	Color chroma;


public:
	void printText(const std::string& text_in, const Vei2& pos, Graphics& gfx, const Color& color) const;
	void printText(const std::string& text_in, const Vei2& pos, Graphics& gfx, const Color& color, const RectI& clippingRect) const;
	void printText(const std::string& text_in, const Vei2& pos, Graphics& gfx, const Color& color, const RectI& clippingRect, float extraSpace) const;
	Font(const std::string& filename, const Color& chroma_in);
	int charHeight;
	int charWidth;
};