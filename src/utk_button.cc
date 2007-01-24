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

bool Button::handle_event(Event *event)
{
	ClickEvent *cev;
	if((cev = dynamic_cast<ClickEvent*>(event))) {
		pressed = false;
		cev->widget = this;

		on_click(cev);
			
		if(callbacks[EVENT_CLICK]) {
			callbacks[EVENT_CLICK](cev);
		}

		return true;
	}

	MButtonEvent *bev;
	if((bev = dynamic_cast<MButtonEvent*>(event))) {
		pressed = bev->pressed;
		return true;
	}

	return false;
}

void Button::draw() const
{
	IVec2 gpos = get_global_pos();

	if(pressed) {
		gfx::color((int)(color.r * 1.25), (int)(color.g * 1.25), (int)(color.b * 1.25), color.a);
	} else {
		gfx::color(color.r, color.g, color.b, color.a);
	}
	gfx::rect(gpos.x, gpos.y, gpos.x + size.x, gpos.y + size.y);

	if(border) {
		gfx::color((int)(color.r * 1.25), (int)(color.g * 1.25), (int)(color.b * 1.25), color.a);

		gfx::line(gpos.x, gpos.y, gpos.x + size.x, gpos.y);
		gfx::line(gpos.x, gpos.y + size.y, gpos.x + size.x, gpos.y + size.y);
		gfx::line(gpos.x, gpos.y, gpos.x, gpos.y + size.y);
		gfx::line(gpos.x + size.x, gpos.y, gpos.x + size.x, gpos.y + size.y);
	}

	if(text) {
		gfx::color(0, 0, 0, color.a);
		gfx::text(gpos.x + border, gpos.y + size.y, text, 18);
	}

	Widget::draw();
}

}	// end of namespace utk
