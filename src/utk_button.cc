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

#include "utk_button.h"
#include "utk_label.h"
#include "utk_gfx.h"

namespace utk {

Button::Button(Widget *widget, Callback cb)
{
	pressed = false;
	flat = false;

	set_min_size(100, 6);

	if (widget) {
		add_child(widget);
	}

	set_callback(EVENT_CLICK, cb);

	set_border(2);

	set_color(128, 100, 80);
}

Button::~Button()
{
}

Widget *Button::handle_event(Event *event)
{
	ClickEvent *cev;
	if((cev = dynamic_cast<ClickEvent*>(event)) && hover && cev->button == MOUSE_LEFT) {
		pressed = false;
		cev->widget = this;

		on_click(cev);
		callback(cev, EVENT_CLICK);

		return this;
	}

	MButtonEvent *bev;
	if((bev = dynamic_cast<MButtonEvent*>(event)) && bev->button == MOUSE_LEFT) {
		if((bev->pressed && hit_test(bev->x, bev->y)) || !bev->pressed) {
			pressed = bev->pressed;
			if(pressed) return this;
		}
	}

	if (dynamic_cast<MMotionEvent*>(event)) {
		event->widget = this;
		return this;
	}

	return 0;
}

void Button::add_child(Widget *w)
{
	Drawable::add_child(w);

	if (dynamic_cast<Drawable*>(w)) {
		set_text(((Drawable*)w)->get_text());
	}

	set_size(w->get_width() + 6, w->get_height() + 6);
}

Widget *Button::get_child_at(int x, int y)
{
	if(!visible) return 0;
	return this;
}

void Button::set_size(int w, int h)
{
	Drawable::set_size(w, h);
	if (child) {
		child->set_pos(size.x/2 - child->get_width()/2, size.y/2 - child->get_height()/2);
	}
}

void Button::draw() const
{
	IVec2 gpos = get_global_pos();
	int r = color.r, g = color.g, b = color.b;

	if (pressed || hover) {
		r += 16;
		g += 16;
		b += 16;
	}
	gfx::color_clamp(r, g, b, color.a);

	if (hover || !flat) {
		gfx::bevel(gpos.x, gpos.y, gpos.x + size.x, gpos.y + size.y, gfx::BEVEL_FILLBG | ((pressed && hover) ? gfx::BEVEL_INSET : 0), 2);
	}

	Widget::draw();
}

void Button::set_text(const char *text)
{
	Drawable::set_text(text);
	if (child) {
		Drawable *dc = dynamic_cast<Drawable*>(child);
		if (dc) dc->set_text(text);
	}
}

void Button::set_flat(bool flat)
{
	this->flat = flat;
}

bool Button::is_flat() const
{
	return flat;
}

Button *create_button(Widget *parent, const char *text, Callback func, void *cdata)
{
	return create_button(parent, new Label(text), func, cdata);
}

Button *create_button(Widget *parent, const char *text, int xsz, int ysz, Callback func, void *cdata)
{
	return create_button(parent, new Label(text), xsz, ysz, func, cdata);
}

Button *create_button(Widget *parent, Widget *child, Callback func, void *cdata)
{
	Button	*bn = new Button(child);
	bn->set_callback(EVENT_CLICK, func, cdata);
	parent->add_child(bn);
	return bn;
}

Button *create_button(Widget *parent, Widget *child, int xsz, int ysz, Callback func, void *cdata)
{
	if (xsz == 0) {
		xsz = child->get_width() + 6;
	}
	if (ysz == 0) {
		ysz = child->get_height() + 6;
	}

	Button	*bn = new Button(child);
	bn->set_callback(EVENT_CLICK, func, cdata);
	bn->set_min_size(xsz, ysz);
	bn->set_size(xsz, ysz);
	parent->add_child(bn);
	return bn;
}

}	// end of namespace utk
