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

#include <vector>
#include <string>
#include <algorithm>
#include <string.h>
#include <errno.h>

#include "ubertk.h"
#include "utk_dialog.h"
#include "utk_common.h"

namespace utk {

Dialog::Dialog()
{
	modal = true;
}
Dialog::~Dialog() {}



Dialog *create_dialog(Widget *parent, int x, int y, int w, int h, const char *title)
{
	Dialog *dlg = new Dialog;
	dlg->set_pos(x, y);
	dlg->set_size(w, h);
	dlg->set_text(title);

	WinFrame *frm = new WinFrame(dlg);
	parent->add_child(frm);

	dlg->rise();
	return dlg;
}

void destroy_dialog(Widget *w)
{
	destroy_window(w);
}


static void def_dialog_handler(Event *event, void *data)
{
	Widget *w = event->widget;
	if(w) w = w->get_window();

	if(w) {
		destroy_window(w);
	}
}

Dialog *message_dialog(const char *msg, unsigned int type, unsigned int bn_mask, Callback func, void *cdata)
{
	static const char *type_str[] = {
		"Question",
		"Info",
		"Warning",
		"Error"
	};
	int lines = 0;
	int max_len = 0;

	if(!func) func = def_dialog_handler;

	Widget *root = get_root_widget();
	VBox *vbox = new VBox;

	char *buf = (char*)malloc(strlen(msg) + 1);
	if(!buf) {
		utk_error("malloc failed");
		delete vbox;
		return 0;
	}

	while(*msg) {
		const char *sptr = msg;
		char *dptr = buf;

		while(*sptr && *sptr != '\n') {
			*dptr++ = *sptr++;
		}
		*dptr = 0;

		msg = *sptr ? sptr + 1 : sptr;

		create_label(vbox, buf);
		
		int len = gfx::text_width(buf, 18);
		if(len > max_len) {
			max_len = len;
		}
		lines++;
	}

	free(buf);

	int bn_width = 0;
	HBox *bn_hbox = create_hbox(vbox);

	if(bn_mask & MSG_BN_OK) {
		create_button(bn_hbox, "OK", func);
	}
	if(bn_mask & MSG_BN_CANCEL) {
		create_button(bn_hbox, "Cancel", func);
	}
	if(bn_mask & MSG_BN_YES) {
		create_button(bn_hbox, "Yes", func);
	}
	if(bn_mask & MSG_BN_NO) {
		create_button(bn_hbox, "No", func);
	}

	bn_width = bn_hbox->get_width();


	int width = MAX(max_len, bn_width) + 10;
	int text_spacing = gfx::text_spacing();
	int height = lines * text_spacing + 10 + (bn_mask ? text_spacing + 10 : 0);
	int x = (root->get_size().x - width) / 2;
	int y = (root->get_size().y - height) / 2;
	
	Dialog *dlg = create_dialog(root, x, y, width, height, type_str[type]);

	vbox->set_size(width - dlg->get_padding()*2, vbox->get_height());
	bn_hbox->set_pos((width - dlg->get_padding()*2)/2 - bn_hbox->get_width()/2, bn_hbox->get_pos().y);
	
	dlg->add_child(vbox);
	dlg->show();

	return dlg;
}

Dialog *message_dialog(const char *msg, unsigned int type, Callback func, void *cdata)
{
	unsigned int bn_mask = (type == MSG_TYPE_QUESTION ? MSG_BN_YES_NO : MSG_BN_OK);
	return message_dialog(msg, type, bn_mask, func, cdata);
}

static void input_dialog_ok_handler(Event *event, void *data)
{
	Widget *w = event->widget;
	if (!w) return;
	Dialog	*dlg = (Dialog*)w->get_window();
	event->widget = dlg->find_widget("entry");
	dlg->callback(event, EVENT_NOTIFY);
}

Dialog *input_dialog(const char *msg, const char *title, const char *deftext, Callback func, void *cdata)
{
	int lines = 0;
	int max_len = 0;

	Widget *root = get_root_widget();
	VBox *vbox = new VBox;

	char *buf = (char*)malloc(strlen(msg) + 1);
	if(!buf) {
		utk_error("malloc failed");
		delete vbox;
		return 0;
	}

	while(*msg) {
		const char *sptr = msg;
		char *dptr = buf;

		while(*sptr && *sptr != '\n') {
			*dptr++ = *sptr++;
		}
		*dptr = 0;

		msg = *sptr ? sptr + 1 : sptr;

		create_label(vbox, buf);
		
		int len = gfx::text_width(buf, 18);
		if(len > max_len) {
			max_len = len;
		}
		lines++;
	}

	free(buf);

	Entry *entry = create_entry(vbox, deftext);
	entry->set_name("entry");

	int bn_width = 0;
	HBox *bn_hbox = create_hbox(vbox);
	create_button(bn_hbox, "OK", input_dialog_ok_handler);
	create_button(bn_hbox, "Cancel", def_dialog_handler);
	bn_width = bn_hbox->get_width();
	
	int width = MAX(max_len, bn_width) + 10;

	entry->set_size(width - 20, entry->get_height());
	entry->set_pos(5, entry->get_pos().y);

	int text_spacing = gfx::text_spacing();
	int height = lines * text_spacing + 40 + entry->get_height();
	int x = (root->get_size().x - width) / 2;
	int y = (root->get_size().y - height) / 2;
	
	Dialog *dlg = create_dialog(root, x, y, width, height, title);

	vbox->set_size(width - dlg->get_padding()*2, vbox->get_height());
	bn_hbox->set_pos((width - dlg->get_padding()*2)/2 - bn_hbox->get_width()/2, bn_hbox->get_pos().y);

	dlg->add_child(vbox);
	dlg->show();
	
	grab_win_focus(entry);
	
	if (func) dlg->set_callback(EVENT_NOTIFY, func, cdata);

	return dlg;
}

} // end of utk namespace
