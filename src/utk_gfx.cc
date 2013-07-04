/*
ubertk is a flexible GUI toolkit targetted towards graphics applications.
Copyright (C) 2007 - 2013 John Tsiombikas <nuclear@member.fsf.org>,
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

#include <stack>
#include <math.h>
#include "utk_gfx.h"
#include "utk_common.h"

#define CIRCLE_SEGMENTS 40

namespace utk {
namespace gfx {

static Color cur_col;

// graphics state
ColorFunc color;
ClipFunc clip;
ImageFunc image;

// drawing calls
RectFunc rect;
LineFunc line;

// text
TextFunc text;
TextSpacingFunc text_spacing;
TextWidthFunc text_width;

// high level drawing functions
void circle(int x1, int y1, int x2, int y2, bool outline)
{
	if (!line) return;
	float xscale = (float) (x2 - x1) / 2.0f;
	float yscale = (float) (y2 - y1) / 2.0f;
	float ang = 2 * 3.14159f / CIRCLE_SEGMENTS;

	if (outline)
	{
		for (unsigned int i=0; i<CIRCLE_SEGMENTS; i++)
		{
			float lx1 = cos(i * ang);
			float ly1 = sin(i * ang);

			float lx2 = cos((i + 1) * ang);
			float ly2 = sin((i + 1) * ang);

			// scale coords
			lx1 = lx1 * xscale + 0.5f + xscale;
			lx2 = lx2 * xscale + 0.5f + xscale;
			ly1 = ly1 * yscale + 0.5f + yscale;
			ly2 = ly2 * yscale + 0.5f + yscale;

			line((int)lx1 + x1, (int)ly1 + y1, (int)lx2 + x1, (int)ly2 + y1, 1);
		}
	}
	else
	{
		int dy = y2 - y1;
		for (int i=0; i<dy; i++)
		{
			float dist = (yscale - i) / yscale;
			float xl = xscale * cos(asin(dist));
			float xstart = xscale - xl + 0.5f;
			float xend = xscale + xl + 0.5f;

			line((int) xstart + x1, i + y1, (int) xend + x1, i + y1, 1);
		}
	}
}

std::stack<Rect> clip_stack;

void push_clip()
{
	if(clip_stack.empty()) {
		clip_stack.push(Rect());
	} else {
		clip_stack.push(clip_stack.top());
	}
}

void pop_clip()
{
	if(!clip_stack.empty()) {
		clip_stack.pop();

		if(clip_stack.empty()) {
			clip(0, 0, 0, 0);
		} else {
			Rect rect = clip_stack.top();
			clip(rect.x1, rect.y1, rect.x2, rect.y2);
		}
	}
}

void set_clip(int x1, int y1, int x2, int y2)
{
	if(clip_stack.empty()) {
		push_clip();
		set_clip(x1, y1, x2, y2);
	} else {
		clip_stack.top() = Rect(x1, y1, x2, y2);
	}
	clip(x1, y1, x2, y2);
}

void mult_clip(int x1, int y1, int x2, int y2)
{
	if(!clip_stack.empty()) {
		Rect cur = clip_stack.top();
		x1 = MAX(cur.x1, x1);
		y1 = MAX(cur.y1, y1);
		x2 = MIN(cur.x2, x2);
		y2 = MIN(cur.y2, y2);

		if(x2 < x1) x2 = x1;
		if(y2 < y1) y2 = y1;
	}
	set_clip(x1, y1, x2, y2);
}

void color_clamp(int r, int g, int b, int a)
{
	cur_col.r = CLAMP(r, 0, 255);
	cur_col.g = CLAMP(g, 0, 255);
	cur_col.b = CLAMP(b, 0, 255);
	cur_col.a = CLAMP(a, 0, 255);
	gfx::color(cur_col.r, cur_col.g, cur_col.b, cur_col.a);
}

void frame(int x1, int y1, int x2, int y2, int thickness)
{
	for(int i=0; i<thickness; i++) {
		gfx::rect(x1 + i, y1 + i, x2 - i, y1 + 1 + i);
		gfx::rect(x1 + i, y1 + i, x1 + 1 + i, y2 - i);
		gfx::rect(x1 + i, y2 - 1 - i, x2 - i, y2 - i);
		gfx::rect(x2 - 1 - i, y1 + i, x2 - i, y2 - i);
	}
}

void bevel(int x1, int y1, int x2, int y2, unsigned int flags, int thickness)
{
	Color light_color = lighter_color(cur_col);
	Color dark_color = darker_color(cur_col);

	if(flags & BEVEL_FILLBG) {
		gfx::color(cur_col.r, cur_col.g, cur_col.b, cur_col.a);
		gfx::rect(x1, y1, x2, y2);
	}

	for(int i=0; i<thickness; i++) {
		if(flags & BEVEL_INSET) {
			gfx::color(dark_color.r, dark_color.g, dark_color.b, dark_color.a);
			gfx::rect(x1 + i, y1 + i, x2 - i, y1 + 1 + i);
			gfx::rect(x1 + i, y1 + i, x1 + 1 + i, y2 - i);

			gfx::color(light_color.r, light_color.g, light_color.b, light_color.a);
			gfx::rect(x1 + i, y2 - 1 - i, x2 - i, y2 - i);
			gfx::rect(x2 - 1 - i, y1 + i, x2 - i, y2 - i);
		} else {
			gfx::color(dark_color.r, dark_color.g, dark_color.b, dark_color.a);
			gfx::rect(x1 + i, y2 - 1 - i, x2 - i, y2 - i);
			gfx::rect(x2 - 1 - i, y1 + i, x2 - i, y2 - i);

			gfx::color(light_color.r, light_color.g, light_color.b, light_color.a);
			gfx::rect(x1 + i, y1 + i, x2 - i, y1 + 1 + i);
			gfx::rect(x1 + i, y1 + i, x1 + 1 + i, y2 - i);
		}
	}
}

void arrow(int x, int y, ArrowOrientation orientation)
{
	int	x1, y1, x2, y2;

	switch (orientation) {
	case ARROW_LEFT:
		{
			y1 = y - 6;
			y2 = y1 + 13;
			x1 = x - 4;
			x2 = x + 3;
			for(int x=x2;x>x1;x--) {
				line(x, y1, x, y2, 1);
				y1++;
				y2--;
			}
		}
		break;

	case ARROW_UP:
		{
			x1 = x - 6;
			x2 = x1 + 13;
			y1 = y - 4;
			y2 = y + 3;
			for (int y=y2;y>y1;y--) {
				line(x1, y, x2, y, 1);
				x1++;
				x2--;
			}
		}
		break;

	case ARROW_RIGHT:
		{
			y1 = y - 6;
			y2 = y1 + 13;
			x1 = x - 3;
			x2 = x + 4;
			for (int x=x1;x<x2;x++) {
				line(x, y1, x, y2, 1);
				y1++;
				y2--;
			}
		}
		break;

	case ARROW_DOWN:
		{
			x1 = x - 6;
			x2 = x1 + 13;
			y1 = y - 2;
			y2 = y + 5;
			for (int y=y1;y<y2;y++) {
				line(x1, y, x2, y, 1);
				x1++;
				x2--;
			}
		}
		break;

	default:
		break;
	}
}

} // end namespace gfx
} // end namespace utk
