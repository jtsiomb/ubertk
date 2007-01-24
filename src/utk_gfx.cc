#include "utk_gfx.h"

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
	}
}
