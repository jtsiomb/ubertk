/*
ubertk is a flexible GUI toolkit targetted towards graphics applications.
Copyright (C) 2007 - 2008 John Tsiombikas <nuclear@member.fsf.org>,
                          Michael Georgoulopoulos <mgeorgoulopoulos@gmail.com>,
				          Kostas Michalopoulos <badsector@slashstone.com>

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
3. The name of the author may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "utk_config.h"
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

IVec2 IVec2::operator-(const IVec2 &v) const
{
	return IVec2(x - v.x, y - v.y);
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

Color lighter_color(const Color &col)
{
	int r = (int)(col.r * 1.25);
	int g = (int)(col.g * 1.25);
	int b = (int)(col.b * 1.25);
	return Color(r > 255 ? 255 : r, g > 255 ? 255 : g, b > 255 ? 255 : b, col.a);
}

Color darker_color(const Color &col)
{
	return Color((int)(col.r * 0.75), (int)(col.g * 0.75), (int)(col.b * 0.75), col.a);
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
