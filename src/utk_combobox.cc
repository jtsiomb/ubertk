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

#include "utk_combobox.h"
#include "utk_arrow.h"
#include "utk_gfx.h"
#include "utk_common.h"

namespace utk {

Container *get_root_widget();

static void reveal_click_handler(Event *event, void *data)
{
	((ComboBox*)data)->show_list();
}

static void list_modify_handler(Event *event, void *data)
{
	((ComboBox*)data)->text_to_selected_item();
}

ComboBox::ComboBox()
{
	entry = 0;
	reveal = 0;

	padding = spacing = 0;

	entry = create_entry(this, "");

	reveal = create_button(this, "");
	reveal->set_min_size(6, 6);
	reveal->set_callback(EVENT_CLICK, reveal_click_handler, this);

	Arrow *arrow = new Arrow(ARROW_DOWN);
	arrow->set_color(Color(80, 64, 64));
	reveal->add_child(arrow);

	reveal->set_size(reveal->get_width(), entry->get_height());

	list_popup = new Popup();
	list_popup->set_padding(3);
	list_popup->hide();
	get_root_widget()->add_child(list_popup);

	list = create_listbox(list_popup, list_popup->get_width() - list_popup->get_padding()*2, list_popup->get_height() - list_popup->get_padding()*2);
	list->set_pos(list_popup->get_padding(), list_popup->get_padding());
	list->set_spacing(0);
	list->set_callback(EVENT_MODIFY, list_modify_handler, this);
}

ComboBox::~ComboBox()
{
	get_root_widget()->remove_child(list_popup);
}

void ComboBox::set_size(int w, int h)
{
	if (entry && reveal) {
		entry->set_size(w - reveal->get_width(), entry->get_height());
	}
	HBox::set_size(w, h);
}

void ComboBox::show_list()
{
	if (list_popup->is_visible()) {
		((Arrow*)reveal->get_child())->set_orientation(ARROW_DOWN);
		list_popup->hide();
		return;
	}

	IVec2	gpos = get_global_pos();
	IVec2	lsize = list->get_child()->get_size();
	int		ytop = gpos.y + reveal->get_height();

	lsize.x += list_popup->get_padding()*2;
	lsize.y += list_popup->get_padding()*2;

	if (lsize.y > 400) lsize.y = 400;
	if (ytop + lsize.y >= get_root_widget()->get_height()) {
		lsize.y = get_root_widget()->get_height() - ytop;
	}
	if (lsize.x < get_width()) lsize.x = get_width();
	if (gpos.x + lsize.x >= get_root_widget()->get_width()) {
		lsize.x = get_root_widget()->get_width() - gpos.x;
	}

	list_popup->set_size(lsize.x, lsize.y);
	list_popup->set_pos(gpos.x, ytop);

	list->set_size(list_popup->get_width() - list_popup->get_padding()*2, list_popup->get_height() - list_popup->get_padding()*2);

	list_popup->show();
	list_popup->rise();

	((Arrow*)reveal->get_child())->set_orientation(ARROW_UP);
}

void ComboBox::add_item(const char *text)
{
	list->add_item(text);
}

void ComboBox::add_item(Widget *widget)
{
	list->add_item(widget);
}

void ComboBox::remove_item(int pos)
{
	return list->remove_item(pos);
}

void ComboBox::select(int sel)
{
	list->select(sel);
}

int ComboBox::get_selected() const
{
	return list->get_selected();
}

const char *ComboBox::get_selected_text() const
{
	return list->get_selected_text();
}

void ComboBox::text_to_selected_item()
{
	entry->set_text(list->get_selected_text());

	Event ev;
	ev.widget = this;
	callback(&ev, EVENT_MODIFY);

	if (list_popup->is_visible()) show_list();
}

void ComboBox::set_readonly(bool readonly)
{
	entry->set_readonly(readonly);
}

bool ComboBox::is_readonly() const
{
	return entry->is_readonly();
}

ComboBox *create_combobox(Widget *parent)
{
	ComboBox	*box = new ComboBox();
	if (parent) parent->add_child(box);
	return box;
}

}	// namespace utk
