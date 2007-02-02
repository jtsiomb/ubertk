#include "utk_button.h"
#include "utk_gfx.h"

namespace utk {

Button::Button(const char *txt, utk::Callback cb)
{
	pressed = false;

	set_text(txt);
	set_callback(EVENT_CLICK, cb);

	set_size(100, gfx::text_spacing() + 4);
	set_border(2);

	set_color(128, 100, 80);
}

Button::~Button()
{
}

Widget *Button::handle_event(Event *event)
{
	ClickEvent *cev;
	if((cev = dynamic_cast<ClickEvent*>(event)) && hit_test(cev->x, cev->y)) {
		pressed = false;
		cev->widget = this;

		on_click(cev);
			
		if(callbacks[EVENT_CLICK]) {
			callbacks[EVENT_CLICK](cev);
		}

		return this;
	}

	MButtonEvent *bev;
	if((bev = dynamic_cast<MButtonEvent*>(event))) {
		if(bev->pressed && hit_test(bev->x, bev->y) || !bev->pressed) {
			pressed = bev->pressed;
			if(pressed) return this;
		}
	}

	return 0;
}

void Button::draw() const
{
	IVec2 gpos = get_global_pos();

	gfx::color(color.r, color.g, color.b, color.a);
	gfx::rect(gpos.x, gpos.y, gpos.x + size.x, gpos.y + size.y);

	if(border) {
		for(int i=0; i<border; i++) {
			if(pressed) {
				gfx::color(light_color.r, light_color.g, light_color.b, light_color.a);
			} else {
				gfx::color(dark_color.r, dark_color.g, dark_color.b, dark_color.a);
			}
			gfx::rect(gpos.x + i, gpos.y + size.y - 1 - i, gpos.x + size.x - i, gpos.y + size.y - i);
			gfx::rect(gpos.x + size.x - 1 - i, gpos.y + i, gpos.x + size.x - i, gpos.y + size.y - i);

			if(pressed) {
				gfx::color(dark_color.r, dark_color.g, dark_color.b, dark_color.a);
			} else {
				gfx::color(light_color.r, light_color.g, light_color.b, light_color.a);
			}
			gfx::rect(gpos.x + i, gpos.y + i, gpos.x + size.x - i, gpos.y + 1 + i);
			gfx::rect(gpos.x + i, gpos.y + i, gpos.x + 1 + i, gpos.y + size.y - i);
		}
	}

	if(text.size()) {
		const char *txt = get_text();
		int twidth = gfx::text_width(txt, 18);

		gfx::color(0, 0, 0, color.a);
		gfx::text(gpos.x + (size.x - twidth) / 2, gpos.y + size.y, get_text(), 18);
	}

	Widget::draw();
}

}	// end of namespace utk
