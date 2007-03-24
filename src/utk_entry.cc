#include <string>
#include "utk_entry.h"
#include "utk_gfx.h"
#include "utk_common.h"

namespace utk {

Entry::Entry(const char *txt, utk::Callback cb)
{
	focused = false;
	cursor = 0;
	set_text(txt);
	set_callback(EVENT_MODIFY, cb);

	set_size(100, gfx::text_spacing() + 4);

	set_color(115, 156, 156);

	vfirst = 0;
}

Entry::~Entry() {}


Widget *Entry::handle_event(Event *event)
{
	KeyboardEvent *kev;
	if((kev = dynamic_cast<KeyboardEvent*>(event))) {
		if(kev->pressed) {
			std::string tmp_str;

			switch(kev->key) {
			case '\b':
				if(!text.empty()) {
					text.erase(text.end() - 1);
				}

			case KEY_LEFT:
				if(vfirst >= (int)text.length() && vfirst > 0) {
					vfirst--;
					std::string view_str(text, vfirst);
					while(vfirst > 0 && gfx::text_width(view_str.c_str(), 18) < size.x) {
						vfirst--;
						view_str = std::string(text, vfirst);
					}
					if(vfirst > 0) vfirst++;
				}
				cursor--;
				if(cursor < 0) cursor = 0;
				break;

				text += "    ";
				cursor += 4;
				break;

			case '\n':
			case '\r':
				break;

			case '\t':
				tmp_str = std::string("    ");
				if(0) {
			default:
					tmp_str = std::string("") + (char)kev->key;
				}
				if(cursor < (int)text.length()) {
					text = std::string(text, 0, cursor) + tmp_str + std::string(text, cursor);
				} else {
					text += tmp_str;
				}
				cursor += tmp_str.length();
			}

			std::string view_str(text, vfirst);
			while(gfx::text_width(view_str.c_str(), 18) > size.x) {
				vfirst++;
				view_str = std::string(text, vfirst);
			}

			on_modify(kev);
			callback(kev, EVENT_MODIFY);
			return this;
		}
	}

	ClickEvent *cev;
	if((cev = dynamic_cast<ClickEvent*>(event)) && hit_test(cev->x, cev->y)) {
		grab_win_focus(this);

		cev->widget = this;

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

	std::string view_text(text, vfirst);

	if(text.length()) {
		gfx::clip(gpos.x + border, gpos.y + border, gpos.x + size.x - border, gpos.y + size.y - border);
		gfx::color(0, 0, 0, color.a);
		gfx::text(gpos.x + border, gpos.y + size.y, view_text.c_str(), 18);
		gfx::clip(0, 0, 0, 0);
	}

	if(focused) {
		int cur_pos = 0;

		if(text.length()) {
			cur_pos = gfx::text_width(std::string(view_text, vfirst, cursor - vfirst).c_str(), 18);
		}

		gfx::clip(gpos.x + border, gpos.y + border, gpos.x + size.x - border, gpos.y + size.y - border);
		gfx::color(64, 64, 64, color.a);
		gfx::line(border + gpos.x + cur_pos, gpos.y + border + 2, border + gpos.x + cur_pos, gpos.y + size.y - border - 2, 2);
		gfx::clip(0, 0, 0, 0);
	}

	Widget::draw();
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
