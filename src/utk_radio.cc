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
// utk_radio.cc

#include "utk_config.h"
#include "utk_radio.h"
#include "utk_container.h"
#include "utk_gfx.h"

namespace utk {

RadioBox::RadioBox(const char *text, utk::Callback cb)
{
	this->text = text;
	if (this->text.size())
	{
		this->size.x = gfx::text_width(text, 18) + gfx::text_spacing();
		this->size.y = gfx::text_spacing();
	}
	else
	{
		size.x = size.y = gfx::text_spacing();
	}
	checked = false;
}
RadioBox::~RadioBox()
{

}

Widget *RadioBox::handle_event(Event *event)
{
	ClickEvent *ce;
	if((ce = dynamic_cast<ClickEvent*>(event)) && hit_test(ce->x, ce->y))
	{
		check();
		return this;
	}

	return 0;
}

void RadioBox::draw() const
{
	IVec2 gpos = get_global_pos();

	// render box
	gfx::color(0, 0, 0, 255);
	gfx::circle(gpos.x, gpos.y, gpos.x + size.y, gpos.y + size.y, false);
	gfx::color(255, 255, 255, 255);
	gfx::circle(gpos.x + 2, gpos.y + 2, gpos.x + size.y - 2, gpos.y + size.y - 2, false);

	// render tick
	if (checked)
	{
		gfx::color(0, 0, 0, 255);
		gfx::circle(gpos.x + 4, gpos.y + 4, gpos.x + size.y - 4, 
			gpos.y + size.y - 4, false);
	}

	// render text
	if (text.size())
	{
		gfx::color(0, 0, 0, 255);
		gfx::text(gpos.x + size.y, gpos.y + size.y, text.c_str(), 18);
	}

	Widget::draw();
}

void RadioBox::check()
{
	Container *cont_par = dynamic_cast<Container*> (parent);
	if (cont_par)
	{
		for (unsigned int i=0; i<cont_par->size(); i++)
		{
			RadioBox *rb = dynamic_cast<RadioBox*> ((*cont_par)[i]);
			if (rb)
			{
				rb->checked = false;
			}
		}
	}
	checked = true;
}

bool RadioBox::is_checked() const
{
	return checked;
}

}
