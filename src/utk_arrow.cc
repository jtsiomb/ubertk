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

#include "utk_arrow.h"

namespace utk {

Arrow::Arrow(ArrowOrientation orientation)
{
	this->orientation = orientation;
	set_size(15, 15);
	set_color(utk::Color(0, 0, 0));
}

Arrow::~Arrow() {}

void Arrow::set_orientation(ArrowOrientation orientation)
{
	this->orientation = orientation;
}

ArrowOrientation Arrow::get_orientation() const
{
	return orientation;
}

void Arrow::draw() const
{
	IVec2	gpos = get_global_pos();

	gfx::color_clamp(color.r, color.g, color.b, color.a);
	gfx::arrow(gpos.x + size.x/2, gpos.y + size.y/2, orientation);
}

Arrow *create_arrow(Widget *parent, ArrowOrientation orientation)
{
	Arrow	*arrow = new Arrow(orientation);
	if (parent) parent->add_child(arrow);
	return arrow;
}

}
