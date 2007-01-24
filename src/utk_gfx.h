#ifndef UBERTK_GFX_H_
#define UBERTK_GFX_H_

#include <utk_types.h>

namespace utk {
	namespace gfx {
		// graphics state
		extern void (*color)(int r, int g, int b, int a);
		extern void (*clip)(int x1, int y1, int x2, int y2);
		extern void (*image)(void *pix, int xsz, int ysz);
		
		// drawing calls
		extern void (*rect)(int x1, int y1, int x2, int y2);
		extern void (*line)(int x1, int y1, int x2, int y2, int width);

		// text
		extern void (*text)(int x, int y, const char *txt, int sz);
		extern int (*text_spacing)(void);
		extern int (*text_width)(const char *txt, int sz);
	}
}

#endif	// UBERTK_GFX_H_
