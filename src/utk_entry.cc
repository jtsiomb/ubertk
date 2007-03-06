#include <string>
#include "utk_entry.h"
#include "utk_gfx.h"
#include "utk_common.h"

namespace utk {

Entry::Entry(const char *txt, utk::Callback cb)
{
	focus = false;
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
			switch(kev->key) {
			case '\b':
				if(!text.empty()) {
					text.erase(text.end() - 1);

					if(vfirst >= (int)text.size() && vfirst > 0) {
						vfirst--;
						std::string view_str(text, vfirst);
						while(vfirst > 0 && gfx::text_width(view_str.c_str(), 18) < size.x) {
							vfirst--;
							view_str = std::string(text, vfirst);
						}
						if(vfirst > 0) vfirst++;
					}
				}
				break;

			case '\t':
				text += "    ";
				break;

			default:
				text += (char)kev->key;
			}

			cursor++;
			
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
		focus = true;
		grab_focus(this);

		on_click(cev);
		callback(cev, EVENT_CLICK);
		return this;
	}
	
	return 0;
}

void Entry::draw() const
{
	IVec2 gpos = get_global_pos();

	int border = focus ? 2 : 1;

	gfx::color(color.r, color.g, color.b, color.a);
	gfx::rect(gpos.x, gpos.y, gpos.x + size.x, gpos.y + size.y);

	gfx::color((int)(color.r * 1.25), (int)(color.g * 1.25), (int)(color.b * 1.25), color.a);
	gfx::line(gpos.x, gpos.y, gpos.x + size.x, gpos.y, border);
	gfx::line(gpos.x, gpos.y + size.y, gpos.x + size.x, gpos.y + size.y, border);
	gfx::line(gpos.x, gpos.y, gpos.x, gpos.y + size.y, border);
	gfx::line(gpos.x + size.x, gpos.y, gpos.x + size.x, gpos.y + size.y, border);

	std::string view_text(text, vfirst);

	if(text.size()) {
		gfx::clip(gpos.x + border, gpos.y + border, gpos.x + size.x - border, gpos.y + size.y - border);
		gfx::color(0, 0, 0, color.a);
		gfx::text(gpos.x + border, gpos.y + size.y, view_text.c_str(), 18);
		gfx::clip(0, 0, 0, 0);
	}

	if(focus) {
		int cur_pos = 0;

		if(text.size()) {
			cur_pos = gfx::text_width(view_text.c_str(), 18);
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
