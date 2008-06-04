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
#include "utk_menus.h"
#include "utk_label.h"
#include "utk_win.h"
#include "utk_gfx.h"
#include "utk_common.h"

namespace utk {

Container *get_root_widget();
unsigned int get_msec();

PopupMenuItem::PopupMenuItem(Widget *widget)
{
	set_color(128, 100, 80);
	submenu = NULL;
	padding = 2;
	this->widget = widget;
	if (widget) {
		add_child(widget);
		set_size(widget->get_width() + padding*2, widget->get_height() + padding*2);
	} else {
		set_size(4, 6);
	}
}

PopupMenuItem::~PopupMenuItem()
{
	if (submenu) {
		get_root_widget()->remove_child(submenu);
	}
}
	
PopupMenuItem *PopupMenuItem::add_item(PopupMenuItem *item, Callback cb)
{
	if (!submenu) {
		submenu = new PopupMenu();
		submenu->master = master;
		get_root_widget()->add_child(submenu);
		size.x += 20;
		((PopupMenu*)(get_parent()->get_parent()))->update_size();
	}
		
	return submenu->add_item(item, cb);
}

PopupMenuItem *PopupMenuItem::add_item(const char *item, Callback cb)
{
	return add_item(new PopupMenuItem(new Label(item)), cb);
}

void PopupMenuItem::add_separator()
{
	add_item(new PopupMenuSeparator());
}

Widget *PopupMenuItem::get_child_at(int x, int y)
{
	return this;
}

void PopupMenuItem::set_size(int w, int h)
{
	Drawable::set_size(w, h);

	if (widget) {
		widget->set_pos(padding, padding);
		widget->set_size(w - padding*2, h - padding*2);
	}
}

Widget *PopupMenuItem::handle_event(Event *event)
{
	MButtonEvent	*bev;
	if ((bev = dynamic_cast<MButtonEvent*>(event))) {
		if (bev->pressed)
			return 0;

		event->widget = this;
		if ((get_msec() - master->showtime < 250))
			return 0;
		on_click(event);
		callback(event, EVENT_CLICK);
		master->cancel();
		return this;
	}
	
	if (widget && (dynamic_cast<MHoverEvent*>(event))) {
		if (((MHoverEvent*)event)->enter) {
			PopupMenu	*popup = (PopupMenu*)get_parent()->get_parent();
			
			if (popup->vis_sub)
				popup->vis_sub->cancel();
				
			if (submenu) {			
				IVec2	gpos = get_global_pos();
				submenu->run(gpos.x + size.x + 4, gpos.y - 6);
				popup->vis_sub = submenu;
			} else {
				popup->vis_sub = 0;
			}

			event->widget = this;
		}
		return this;
	}
	
	return 0;
}

void PopupMenuItem::draw() const
{
	IVec2	gpos = get_global_pos();
	bool	barr = false;
	
	if (hover || (submenu && submenu->is_visible())) {
		if (hover)
			gfx::color_clamp(color.r + 16, color.g + 16, color.b + 16, color.a);
		else
			gfx::color_clamp(color.r, color.g, color.b, color.a);
		gfx::bevel(gpos.x, gpos.y, gpos.x + size.x, gpos.y + size.y, gfx::BEVEL_FILLBG, 1);
		barr = true;
	}
	
	if (submenu) {
		int	y1 = gpos.y + size.y/2 - 6;
		int	y2 = y1 + 13;
		if (barr)
			gfx::color(80, 64, 64, 255);
		else
			gfx::color(64, 64, 80, 255);
		for (int x=gpos.x + size.x - 10;y1 <= y2;x++) {
			gfx::line(x, y1, x, y2, 1);
			y1++;
			y2--;
		}
	}

	Widget::draw();
}


PopupMenuSeparator::PopupMenuSeparator()
	: PopupMenuItem(NULL)
{
	set_color(140, 140, 150);
}

void PopupMenuSeparator::draw() const
{
	IVec2	gpos = get_global_pos();

	gfx::color_clamp(color.r, color.g, color.b, color.a);
	gfx::bevel(gpos.x, gpos.y + size.y/2 - 1, gpos.x + size.x, gpos.y + size.y/2 + 1, gfx::BEVEL_INSET, 1);
	
	Widget::draw();
}


PopupMenu::PopupMenu()
{
	delete_master = true;

	vbox = create_vbox(this, 0, 0);
	master = 0;
	vis_sub = 0;
}

PopupMenu::~PopupMenu()
{
}

void PopupMenu::update_size()
{
	int	max = 0;
	vbox->layout();
	for (int i=0; i<vbox->size(); i++) {
		Widget	*c = (Widget*)(*vbox)[i];
		if (max < c->get_width())
			max = c->get_width();
	}
	max += padding*2;
	
	set_size(max, vbox->get_height() + padding*2);
}

void PopupMenu::set_size(int w, int h)
{
	Popup::set_size(w, h);

	for (int i=0; i<vbox->size(); i++) {
		Widget	*c = (Widget*)(*vbox)[i];
		c->set_size(w - padding*2, c->get_height());
	}
	vbox->layout();
}

Widget *PopupMenu::handle_event(Event *event)
{
	FocusEvent	*fev;
	if ((fev = dynamic_cast<FocusEvent*>(event))) {
		if ((dynamic_cast<WinFrame*>(event->widget)) && !fev->focus && is_visible()) {
			master->cancel();
		}
		return this;
	}
	
	return 0;
}

PopupMenuItem *PopupMenu::add_item(PopupMenuItem *item, Callback cb)
{
	if (!master) {
		master = this;
	}
	
	if (cb) {
		item->set_callback(EVENT_CLICK, cb);
	}

	item->master = master;
	vbox->add_child(item);
	update_size();
	return item;
}

PopupMenuItem *PopupMenu::add_item(const char *item, Callback cb)
{
	return add_item(new PopupMenuItem(new Label(item)), cb);
}

void PopupMenu::add_separator()
{
	add_item(new PopupMenuSeparator());
}

void PopupMenu::run(int x, int y)
{
	if (is_visible()) return;
	
	if (x == -1) x = get_mouse_pos().x - 10;
	if (y == -1) y = get_mouse_pos().y - 10;
	
	if (x + size.x >= get_root_widget()->get_width()) {
		x = get_root_widget()->get_width() - size.x;
	}
	if (y + size.y >= get_root_widget()->get_height()) {
		y = get_root_widget()->get_height() - size.y;
	}
	
	set_pos(x, y);
	show();
	rise();
	grab_focus(this);
	showtime = get_msec();
}

void PopupMenu::cancel()
{
	if (!is_visible())
		return;
	if (vis_sub)
		vis_sub->cancel();
	vis_sub = 0;
	hide();
	if (master == this && delete_master) {
		get_root_widget()->remove_child(this);
	}
}

PopupMenu *create_popup_menu(bool show)
{
	PopupMenu	*m = new PopupMenu();
	get_root_widget()->add_child(m);
	if (show) m->run();
	return m;
}

}
