#include "utk_types.h"

namespace utk {

IVec2::IVec2(int x, int y)
{
	this->x = x;
	this->y = y;
}

IVec2 IVec2::operator+(const IVec2 &v) const
{
	return IVec2(x + v.x, y + v.y);
}

bool rect_test(const IVec2 &tl, const IVec2 &br, const IVec2 &pos)
{
	if (pos.x < tl.x) return false;
	if (pos.y < tl.y) return false;
	if (pos.x > br.x) return false;
	if (pos.y > br.y) return false;
	return true;
}

Color::Color(int r, int g, int b, int a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

Rect::Rect()
{
	x1 = x2 = y1 = y2 = 0;
}

Rect::Rect(int x1, int y1, int x2, int y2)
{
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
}

}
