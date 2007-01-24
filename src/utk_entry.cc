#include <string>
#include "utk_entry.h"
#include "utk_gfx.h"

namespace utk {

Entry::Entry(const char *txt, utk::Callback cb)
{
	focus = false;
	set_text(txt);
	set_callback(EVENT_MODIFY, cb);

	set_size(100, gfx::text_spacing() + 4);

	set_color(115, 156, 156);
}

Entry::~Entry() {}

Widget *Entry::handle_event(Event *event)
{
	KeyboardEvent *kev;
	if((kev = dynamic_cast<KeyboardEvent*>(event))) {
		if(kev->pressed) {
			std::string str = std::string(text ? text : "") + (char)kev->key;
			set_text(str.c_str());

			cursor++;

			/*
			char *first_ptr = text + cursor;
			while(first_ptr > text && gfx::text_width(first_ptr, 18) < get_width() - border * 2) {
				first_ptr--;
			}
			if(first_ptr > text) {
				first_ptr++;
			}
			*/

			on_modify(kev);

			if(callbacks[EVENT_MODIFY]) {
				callbacks[EVENT_MODIFY](kev);
			}
			
			return this;
		}
	}

	ClickEvent *cev;
	if((cev = dynamic_cast<ClickEvent*>(event))) {
		focus = true;
		grab_focus(this);

		on_click(cev);

		if(callbacks[EVENT_CLICK]) {
			callbacks[EVENT_CLICK](cev);
		}

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

	if(text) {
		gfx::color(0, 0, 0, color.a);
		gfx::text(gpos.x + border, gpos.y + size.y, text, 18);
	}

	if(focus) {
		int cur_pos = 0;

		if(text) {
			/*char *cur_ptr = text + cursor;
			char savec = *cur_ptr;
			*cur_ptr = 0;*/
			cur_pos = gfx::text_width(text, 18);
			//*cur_ptr = savec;
		}

		gfx::color(64, 64, 64, color.a);
		gfx::line(border + gpos.x + cur_pos, gpos.y + border + 2, border + gpos.x + cur_pos, gpos.y + size.y - border - 2, 2);
	}

	Widget::draw();
}

void Entry::on_click(Event *event) {}
void Entry::on_modify(Event *event) {}

}	// utk namespace end
