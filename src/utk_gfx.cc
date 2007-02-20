#include <stack>
#include <math.h>
#include "utk_gfx.h"
#include "utk_common.h"

#define CIRCLE_SEGMENTS 40

namespace utk {
namespace gfx {

static Color cur_col;

void (*color)(int r, int g, int b, int a) = 0;
void (*clip)(int x1, int y1, int x2, int y2) = 0;
void (*image)(int x, int y, void *pix, int xsz, int ysz) = 0;

void (*rect)(int x1, int y1, int x2, int y2) = 0;
void (*line)(int x1, int y1, int x2, int y2, int border) = 0;
void (*text)(int x, int y, const char *txt, int sz) = 0;
int (*text_spacing)(void) = 0;
int (*text_width)(const char *txt, int sz) = 0;
		
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
	
} // end namespace gfx
} // end namespace utk
