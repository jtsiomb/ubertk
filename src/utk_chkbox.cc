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
// utk_chkbox.cc

#include <stdio.h>
#include "utk_config.h"
#include "utk_chkbox.h"
#include "utk_gfx.h"

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

namespace utk {

CheckBox::CheckBox(const char *text)
{
	this->text = std::string(text);
	if(text) {
		this->size.x = gfx::text_width(text, 18) + gfx::text_spacing();
		this->size.y = gfx::text_spacing();
	} else {
		size.x = size.y = gfx::text_spacing();
	}
	checked = false;
}

CheckBox::~CheckBox() {}

Widget *CheckBox::handle_event(Event *event)
{
	ClickEvent *ce;
	if((ce = dynamic_cast<ClickEvent*>(event)) && hit_test(ce->x, ce->y))
	{
		checked = !checked;

		on_modify(event);
		callback(event, EVENT_MODIFY);

		return ce->widget = this;
	}

	return 0;
}

void CheckBox::draw() const
{
	IVec2 gpos = get_global_pos();

	// render box
	gfx::color(0, 0, 0, 255);
	gfx::rect(gpos.x, gpos.y, gpos.x + size.y, gpos.y + size.y);
	gfx::color(255, 255, 255, 255);
	gfx::rect(gpos.x + 2, gpos.y + 2, gpos.x + size.y - 2, gpos.y + size.y - 2);

	// render tick
	if (checked)
	{
		gfx::color(255, 100, 100, 255);
		gfx::line(gpos.x + 4, gpos.y + 4, gpos.x + size.y - 4,
			gpos.y + size.y - 4, 5);
		gfx::line(gpos.x + size.y - 4, gpos.y + 4, gpos.x + 4,
			gpos.y + size.y - 4, 5);
	}

	// render text
	if (text.size())
	{
		gfx::color(0, 0, 0, 255);
		gfx::text(gpos.x + size.y, gpos.y + size.y, text.c_str(), 18);
	}

	Widget::draw();
}

void CheckBox::check()
{
	checked = true;
}

void CheckBox::uncheck()
{
	checked = false;
}

void CheckBox::set_checked(bool checked)
{
	this->checked = checked;
}

bool CheckBox::is_checked() const
{
	return checked;
}

void CheckBox::on_modify(Event *event)
{
	if(link_flt) {
		*link_flt = checked ? 1.0f : 0.0f;
	}
	if(link_int) {
		*link_int = (int)checked;
	}
	if(link_str) {
		snprintf(link_str, link_str_width, "%s", checked ? "true" : "false");
	}
	if(link_bool) {
		*link_bool = checked;
	}
}


CheckBox *create_checkbox(Widget *parent, const char *text, bool checked, Callback func, void *cdata)
{
	CheckBox *cbox = new CheckBox(text);
	cbox->set_checked(checked);
	cbox->set_callback(EVENT_MODIFY, func, cdata);
	parent->add_child(cbox);
	return cbox;
}

CheckBox *create_checkbox(Widget *parent, const char *text, bool checked, bool *link)
{
	CheckBox *cbox = create_checkbox(parent, text, checked);
	cbox->set_link(link);
	return cbox;
}

void destroy_checkbox(CheckBox *cbox)
{
	delete cbox;
}


} // end namespace utk
