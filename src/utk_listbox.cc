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
#include "utk_config.h"
#include <string.h>
#include "utk_listbox.h"
#include "utk_label.h"
#include "utk_gfx.h"
#include "utk_common.h"

namespace utk {

ListBox::ListBox()
{
	vbox = new VBox;
	add_child(vbox);
	sel = -1;
}

ListBox::~ListBox()
{
	//delete vbox;	// vbox will delete its children in herod mode (default)
}

Widget *ListBox::handle_event(Event *event)
{
	// handle keyboard up/down for selection change
	KeyboardEvent *kev;
	if((kev = dynamic_cast<KeyboardEvent*>(event)) && kev->pressed) {
		switch(kev->key) {
		case KEY_UP:
			if(sel > 0) {
				select(sel - 1);
			}
			kev->widget = this;
			return this;

		case KEY_DOWN:
			if(sel < (int)vbox->size() - 1) {
				select(sel + 1);
			}
			kev->widget = this;
			return this;
		}

		return 0;
	}

	// grab the window focus if a mouse button is pressed over the listbox
	MButtonEvent *bev;
	if((bev = dynamic_cast<MButtonEvent*>(event)) && hit_test(bev->x, bev->y)
			&& !hbar->hit_test(bev->x, bev->y) && !vbar->hit_test(bev->x, bev->y)
			&& bev->pressed && !bev->widget && bev->button == MOUSE_LEFT)
	{
		grab_win_focus(this);
		return this;
	}

	// select by clicking
	ClickEvent *cev;
	if((cev = dynamic_cast<ClickEvent*>(event)) && hit_test(cev->x, cev->y) && !cev->widget && cev->button == MOUSE_LEFT) {
		int i = 0;
		VBox::iterator iter = vbox->begin();
		while(iter != vbox->end()) {
			if((*iter)->hit_test((*iter)->get_global_pos().x, cev->y)) {
				select(i);

				cev->widget = this;
				return this;
			}
			iter++;
			i++;
		}
	}
	
	return ScrollWin::handle_event(event);
}

void ListBox::add_item(Widget *item, int pos)
{
	if(pos < 0 || pos > (int)vbox->size()) {
		vbox->add_child(item);
	} else {
		// TODO: insert in specific position
	}
}

void ListBox::add_item(const char *str, int pos)
{
	add_item(new Label(str), pos);
}

void ListBox::remove_item(int pos)
{
	// TODO
}

void ListBox::select(int pos)
{
	if(pos < 0) pos = -1;
	if(pos >= (int)vbox->size()) {
		pos = vbox->size() - 1;
	}

	sel = pos;

	Event ev;
	ev.widget = this;
	callback(&ev, EVENT_MODIFY);
	on_modify(&ev);

	// TODO: do something to keep the selection on screen ...
}

void ListBox::select(const char *str)
{
	for(size_t i=0; i<vbox->size(); i++) {
		Drawable *w = dynamic_cast<Drawable*>((*vbox)[i]);
		if(w && strcmp(w->get_text(), str) == 0) {
			select(i);
			break;
		}
	}
	select(-1);
}

int ListBox::get_selected() const
{
	return sel;
}

const char *ListBox::get_selected_text() const
{
	Drawable *w;
	if((w = dynamic_cast<Drawable*>((*vbox)[sel]))) {
		return w->get_text();
	}
	return 0;
}

void ListBox::set_spacing(int sp)
{
	vbox->set_spacing(sp);
}

int ListBox::get_spacing() const
{
	return vbox->get_spacing();
}

void ListBox::clear()
{
	vbox->clear();
}

void ListBox::draw() const
{
	ScrollWin::draw();

	if(sel >= 0) {
		bool hs = check_hscroll_vis();
		bool vs = check_vscroll_vis();
		IVec2 gpos = get_global_pos();

		gfx::push_clip();
		gfx::mult_clip(gpos.x, gpos.y, gpos.x + size.x - (vs ? vbar->get_size().x : 0), gpos.y + size.y - (hs ? hbar->get_size().y : 0));

		int i = 0;
		VBox::iterator iter = vbox->begin();
		while(iter != vbox->end()) {
			if(i++ == sel) {
				IVec2 gpos = (*iter)->get_global_pos();
				IVec2 sz = (*iter)->get_size();

				gfx::color_clamp(80, 120, 255, 64);
				gfx::rect(gpos.x, gpos.y, gpos.x + vbox->get_size().x, gpos.y + sz.y);
				break;
			}
			iter++;
		}

		gfx::pop_clip();
	}
}

ListBox *create_listbox(Widget *parent, int width, int height, Callback func, void *cdata)
{
	ListBox *lbox = new ListBox;
	lbox->set_callback(EVENT_MODIFY, func, cdata);
	lbox->set_size(width, height);
	parent->add_child(lbox);
	return lbox;
}

ListBox *create_listbox(Widget *parent, int width, int height, const char **items, int inum, Callback func, void *cdata)
{
	ListBox *lbox = create_listbox(parent, width, height, func, cdata);

	for(int i=0; i<inum; i++) {
		lbox->add_item(items[i]);
	}
	return lbox;
}

ListBox *create_listbox(Widget *parent, int width, int height, int *link)
{
	// TODO
	return 0;
}

ListBox *create_listbox(Widget *parent, int width, int height, const char *link)
{
	// TODO
	return 0;
}

void destroy_listbox(ListBox *lb)
{
	if(dynamic_cast<ListBox*>(lb)) {
		delete lb;
	} else {
		utk_error("non-listbox widget passed to destroy_listbox()\n");
	}
}


} // end of namespace utk
