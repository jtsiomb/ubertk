#include "utk_gfx.h"
#include <math.h>

#define CIRCLE_SEGMENTS 40

namespace utk {
namespace gfx {

void (*color)(int r, int g, int b, int a) = 0;
void (*clip)(int x1, int y1, int x2, int y2) = 0;
void (*image)(void *pix, int xsz, int ysz) = 0;

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
		for (unsigned int i=0; i<dy; i++)
		{
			float dist = (yscale - i) / yscale;
			float xl = xscale * cos(asin(dist));
			float xstart = xscale - xl + 0.5f;
			float xend = xscale + xl + 0.5f;

			line((int) xstart + x1, i + y1, (int) xend + x1, i + y1, 1);
		}
	}
}

	
} // end namespace gfx
} // end namespace utk
