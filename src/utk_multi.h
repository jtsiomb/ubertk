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

// utk_multi.h

#ifndef _UTK_MULTI_H_
#define _UTK_MULTI_H_

#include "ubertk.h"

namespace utk
{

	class UTKContext
	{
	protected:
		Container *root;
		Container *old_root;
		void *old_user_data;

		// saved gfx functions
		gfx::ColorFunc old_color_func;
		gfx::ClipFunc old_clip_func;
		gfx::ImageFunc old_image_func;
		gfx::RectFunc old_rect_func;
		gfx::LineFunc old_line_func;
		gfx::TextFunc old_text_func;
		gfx::TextSpacingFunc old_text_spacing_func;
		gfx::TextWidthFunc old_text_width_func;

		void begin();
		void end();		

	public:
		UTKContext(int w, int h);
		virtual ~UTKContext();

		// graphics handlers
		virtual void color(int r, int g, int b, int a) = 0;
		virtual void clip(int x1, int y1, int x2, int y2) = 0;
		virtual void image(int x, int y, const void *pix, int xsz, int ysz) = 0;
		virtual void rect(int x1, int y1, int x2, int y2) = 0;
		virtual void line(int x1, int y1, int x2, int y2, int width) = 0;
		virtual void text(int x, int y, const char *txt, int sz) = 0;
		virtual int text_spacing() = 0;
		virtual int text_width(const char *txt, int sz) = 0;

		void draw();
	};

} // end namespace utk

#endif // ndef _UTK_MULTI_H_