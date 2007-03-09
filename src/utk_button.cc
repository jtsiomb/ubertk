#include "utk_button.h"
#include "utk_gfx.h"

namespace utk {

Button::Button(const char *txt, Callback cb)
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
		callback(cev, EVENT_CLICK);

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

	gfx::color_clamp(color.r, color.g, color.b, color.a);
	gfx::bevel(gpos.x, gpos.y, gpos.x + size.x, gpos.y + size.y, gfx::BEVEL_FILLBG | (pressed ? gfx::BEVEL_INSET : 0), 2);

	if(text.size()) {
		const char *txt = get_text();
		int twidth = gfx::text_width(txt, 18);

		gfx::color(0, 0, 0, color.a);
		gfx::text(gpos.x + (size.x - twidth) / 2, gpos.y + size.y, txt, 18);
	}

	Widget::draw();
}

Button *create_button(Widget *parent, const char *text, Callback func, void *cdata)
{
	return create_button(parent, text, 100, gfx::text_spacing() + 4, func, cdata);
}

Button *create_button(Widget *parent, const char *text, int xsz, int ysz, Callback func, void *cdata)
{
	if(ysz == 0) {
		ysz = gfx::text_spacing() + 4;
	}

	Button *bn = new Button(text);
	bn->set_callback(EVENT_CLICK, func, cdata);
	bn->set_size(xsz, ysz);
	parent->add_child(bn);
	return bn;
}

}	// end of namespace utk
