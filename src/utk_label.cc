/*
ubertk is a flexible GUI toolkit targetted towards graphics applications.
Copyright (C) 2007 - 2018 John Tsiombikas <nuclear@member.fsf.org>,
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

#include "utk_label.h"
#include "utk_gfx.h"

namespace utk {

Label::Label(const char *txt)
{
	if(txt) set_text(txt);
	text_color = Color(0, 0, 0, 255);
	set_size(txt ? gfx::text_width(txt, 18) : 0, gfx::text_spacing());
}

Label::~Label() {}

void Label::set_text(const char *txt)
{
	set_size(txt ? gfx::text_width(txt, 18) : 0, gfx::text_spacing());
	Drawable::set_text(txt);
}

void Label::set_text_color(int r, int g, int b, int a)
{
	text_color = Color(r, g, b, a);
}

void Label::set_text_color(const Color &col)
{
	text_color = col;
}

const Color &Label::get_text_color() const
{
	return text_color;
}

void Label::draw() const
{
	if(text.size()) {
		IVec2 gpos = get_global_pos();
		gfx::color(text_color.r, text_color.g, text_color.b, text_color.a);
		gfx::text(gpos.x, gpos.y + gfx::text_spacing() + 1, text.c_str(), 18);
	}
}

Label *create_label(Widget *parent, const char *txt)
{
	Label *lb = new Label(txt);
	parent->add_child(lb);
	return lb;
}

}
