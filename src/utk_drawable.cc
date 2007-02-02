#include <stdlib.h>
#include <string.h>
#include "utk_drawable.h"

namespace utk {

Drawable::Drawable()
{
	border = 0;
}

Drawable::~Drawable() {}

void Drawable::set_text(const char *str)
{
	text = std::string(str ? str : "");
}

const char *Drawable::get_text() const
{
	return text.c_str();
}
	

void Drawable::set_color(int r, int g, int b, int a)
{
	color = Color(r, g, b, a);
	light_color = lighter_color(color);
	dark_color = darker_color(color);
}

#define MIN(a, b)	((a) < (b) ? (a) : (b))
void Drawable::set_color(const Color &col)
{
	color = col;
	light_color = lighter_color(col);
	dark_color = darker_color(col);
}

const Color &Drawable::get_color() const
{
	return color;
}

void Drawable::set_alpha(int a)
{
	color.a = light_color.a = dark_color.a = a;
}

int Drawable::get_alpha() const
{
	return color.a;
}

void Drawable::set_border(int border)
{
	this->border = border;
}

int Drawable::get_border() const
{
	return border;
}

}	// end of utk namespace
