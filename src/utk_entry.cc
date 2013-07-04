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

#include <string>
#include "utk_entry.h"
#include "utk_gfx.h"
#include "utk_common.h"

namespace utk {

Entry::Entry(const char *txt, utk::Callback cb)
{
	focused = false;
	cursor = 0;
	readonly = false;

	set_text(txt);
	set_callback(EVENT_MODIFY, cb);

	set_size(100, gfx::text_spacing() + 4);

	set_color(115, 156, 156);

	vscroll = 0;
}

Entry::~Entry() {}

int Entry::cursor_from_x(int x) const
{
	if (x < 0) return -1;

	int			chw;
	const char	*txt = get_text();
	char		chs[2];
	chs[1] = 0;
	for (int i=0; *txt; i++, txt++) {
		int	cx = gfx::text_width(text.substr(0, i).c_str(), 18) - vscroll;
		chs[0] = *txt;
		chw = gfx::text_width(chs, 18);
		if (x >= cx && x < cx + chw) {
			return i;
		}
	}

	return 2147483640;
}

int Entry::x_from_cursor(int c) const
{
	if (c < 0 || c > (int)text.length()) return 0;

	return gfx::text_width(text.substr(0, c).c_str(), 18) - vscroll;
}

void Entry::scroll_to_cursor()
{
	while (true) {
		int	cx = x_from_cursor(cursor);
		if (cx < 0) {
			if (vscroll > 0) {
				vscroll -= size.x/2;
			}
		} else if (cx >= size.x - 4) {
			vscroll += 20;
		} else break;
	}

	if (vscroll < 0) vscroll = 0;
}

Widget *Entry::handle_event(Event *event)
{
	KeyboardEvent *kev;
	if((kev = dynamic_cast<KeyboardEvent*>(event))) {
		if(kev->pressed) {
			std::string tmp_str;

			switch(kev->key) {
			case '\b':
				if(!readonly && !text.empty()) {
					text.erase(text.end() - 1);
					cursor--;
				}
				break;

			case KEY_DELETE:
				if (readonly || text.empty()) break;

				if (cursor < (int)text.size()) {
					text = text.substr(0, cursor) + text.substr(cursor + 1, text.size());
				}

				break;

			case KEY_LEFT:
				cursor--;
				if(cursor < 0) cursor = 0;
				break;

			case KEY_RIGHT:
				cursor++;
				if(cursor > (int)text.length()) cursor = (int)text.length();
				break;

			case KEY_HOME:
				vscroll = 0;
				cursor = 0;
				break;

			case KEY_END:
				vscroll = 0;
				cursor = (int)text.length();
				break;

			case '\n':
			case '\r':
				event->widget = this;
				callback(event, EVENT_NOTIFY);
				break;

			case '\t':
				tmp_str = std::string("    ");
				if(0) {
			default:
					tmp_str = std::string("") + (char)kev->key;
				}

				if (readonly) break;

				// filter out non-character keys
				if (kev->key != '\t' && (kev->key < 32 || kev->key > 126))
					break;

				if(cursor < (int)text.length()) {
					text = std::string(text, 0, cursor) + tmp_str + std::string(text, cursor);
				} else {
					text += tmp_str;
				}
				cursor += (int)tmp_str.length();
			}

			scroll_to_cursor();

			on_modify(kev);
			callback(kev, EVENT_MODIFY);
			return this;
		}
	}

	ClickEvent *cev;
	if((cev = dynamic_cast<ClickEvent*>(event)) && hit_test(cev->x, cev->y)) {
		grab_win_focus(this);

		cev->widget = this;

		cursor = cursor_from_x(cev->x - get_global_pos().x - border);
		if (cursor < 0) {
			cursor = 0;
		} else if (cursor > (int)text.length()) {
			cursor = (int)text.length();
		}
		scroll_to_cursor();

		on_click(cev);
		callback(cev, EVENT_CLICK);
		return this;
	}

	return 0;
}

void Entry::draw() const
{
	IVec2 gpos = get_global_pos();

	int border = focused ? 2 : 1;

	gfx::color_clamp(color.r, color.g, color.b, color.a);
	gfx::bevel(gpos.x, gpos.y, gpos.x + size.x, gpos.y + size.y, gfx::BEVEL_INSET | gfx::BEVEL_FILLBG, border);

	std::string view_text(text);

	if(text.length()) {
		gfx::clip(gpos.x + border, gpos.y + border, gpos.x + size.x - border, gpos.y + size.y - border);
		gfx::color(0, 0, 0, color.a);
		gfx::text(gpos.x + border - vscroll, gpos.y + size.y, view_text.c_str(), 18);
		gfx::clip(0, 0, 0, 0);
	}

	if(focused) {
		int cur_pos = 0;

		if(text.length()) {
			cur_pos = x_from_cursor(cursor);
		}

		gfx::clip(gpos.x + border, gpos.y + border, gpos.x + size.x - border, gpos.y + size.y - border);
		gfx::color(64, 64, 64, color.a);
		gfx::line(border + gpos.x + cur_pos + 1, gpos.y + border + 2, border + gpos.x + cur_pos + 1, gpos.y + size.y - border - 2, 2);
		gfx::clip(0, 0, 0, 0);
	}

	Widget::draw();
}

void Entry::set_readonly(bool readonly)
{
	this->readonly = true;
}

bool Entry::is_readonly() const
{
	return readonly;
}


Entry *create_entry(Widget *parent, const char *text, int width, Callback func, void *cdata)
{
	Entry *en = new Entry(text);
	en->set_callback(EVENT_MODIFY, func, cdata);
	en->set_size(width, gfx::text_spacing() + 4);
	parent->add_child(en);
	return en;
}

void destroy_entry(Entry *en)
{
	if(dynamic_cast<Entry*>(en)) {
		delete en;
	} else {
		utk_error("non-entry widget passed to destroy_entry()\n");
	}
}

}	// utk namespace end
