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
#ifndef UBERTK_GFX_H_
#define UBERTK_GFX_H_

#include <utk_types.h>

namespace utk {
	namespace gfx {
		// graphics state
		extern void (*color)(int r, int g, int b, int a);
		extern void (*clip)(int x1, int y1, int x2, int y2);
		extern void (*image)(int x, int y, const void *pix, int xsz, int ysz);
		
		// drawing calls
		extern void (*rect)(int x1, int y1, int x2, int y2);
		extern void (*line)(int x1, int y1, int x2, int y2, int width);

		// text
		extern void (*text)(int x, int y, const char *txt, int sz);
		extern int (*text_spacing)(void);
		extern int (*text_width)(const char *txt, int sz);

		// ---- high level drawing functions ----
		void circle(int x1, int y1, int x2, int y2, bool outline);
		
		void push_clip();
		void pop_clip();
		void set_clip(int x1, int y1, int x2, int y2);
		void mult_clip(int x1, int y1, int x2, int y2);

		void color_clamp(int r, int g, int b, int a);

		void frame(int x1, int y1, int x2, int y2, int thickness = 1);

		enum {BEVEL_INSET = 1, BEVEL_FILLBG = 2};
		void bevel(int x1, int y1, int x2, int y2, unsigned int flags = 0, int thickness = 1);
	}
}

#endif	// UBERTK_GFX_H_
