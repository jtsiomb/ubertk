#include "utk_button.h"
#include "utk_label.h"
#include "utk_gfx.h"

namespace utk {

Button::Button(Widget *widget, Callback cb)
{
	pressed = false;
	flat = false;

	if (widget->get_parent() != this) {
		add_child(widget);
	}

	if (dynamic_cast<Drawable*>(widget)) {
		set_text(((Drawable*)widget)->get_text());
	}
	set_callback(EVENT_CLICK, cb);
	
	set_min_size(100, 6);

	set_size(widget->get_width() + 6, widget->get_height() + 6);
	set_border(2);

	set_color(128, 100, 80);
}

Button::~Button()
{
}

Widget *Button::handle_event(Event *event)
{
	ClickEvent *cev;
	if((cev = dynamic_cast<ClickEvent*>(event)) && hover && cev->button == MOUSE_LEFT) {
		pressed = false;
		cev->widget = this;

		on_click(cev);
		callback(cev, EVENT_CLICK);

		return this;
	}

	MButtonEvent *bev;
	if((bev = dynamic_cast<MButtonEvent*>(event)) && bev->button == MOUSE_LEFT) {
		if(bev->pressed && hit_test(bev->x, bev->y) || !bev->pressed) {
			pressed = bev->pressed;
			if(pressed) return this;
		}
	}
	
	if (dynamic_cast<MMotionEvent*>(event)) {
		event->widget = this;
		return this;
	}

	return 0;
}

Widget *Button::get_child_at(int x, int y)
{
	return this;
}

void Button::set_size(int w, int h)
{
	Drawable::set_size(w, h);
	child->set_pos(size.x/2 - child->get_width()/2, size.y/2 - child->get_height()/2);
}

void Button::draw() const
{
	IVec2 gpos = get_global_pos();
	int r = color.r, g = color.g, b = color.b;

	if (pressed || hover) {
		r += 16;
		g += 16;
		b += 16;
	}
	gfx::color_clamp(r, g, b, color.a);

	if (hover || !flat) {
		gfx::bevel(gpos.x, gpos.y, gpos.x + size.x, gpos.y + size.y, gfx::BEVEL_FILLBG | ((pressed && hover) ? gfx::BEVEL_INSET : 0), 2);
	}

	/*if(text.size()) {
		const char *txt = get_text();
		int twidth = gfx::text_width(txt, 18);

		gfx::color(0, 0, 0, color.a);
		gfx::text(gpos.x + (size.x - twidth) / 2, gpos.y + size.y, txt, 18);
	}*/

	Widget::draw();
}

void Button::set_text(const char *text)
{
	Drawable	*dc = dynamic_cast<Drawable*>(child);
	if (dc) dc->set_text(text);
	Drawable::set_text(text);
}

void Button::set_flat(bool flat)
{
	this->flat = flat;
}

bool Button::is_flat() const
{
	return flat;
}

Button *create_button(Widget *parent, const char *text, Callback func, void *cdata)
{
	return create_button(parent, new Label(text), func, cdata);
}

Button *create_button(Widget *parent, const char *text, int xsz, int ysz, Callback func, void *cdata)
{
	if(ysz == 0) {
		ysz = gfx::text_spacing() + 4;
	}

	return create_button(parent, new Label(text), xsz, ysz, func, cdata);
}

Button *create_button(Widget *parent, Widget *child, Callback func, void *cdata)
{
	return create_button(parent, child, child->get_width() + 6, 0, func, cdata);
}

Button *create_button(Widget *parent, Widget *child, int xsz, int ysz, Callback func, void *cdata)
{
	if (ysz == 0) {
		ysz = child->get_height() + 6;
	}

	Button	*bn = new Button(child);
	bn->set_callback(EVENT_CLICK, func, cdata);
	bn->set_size(xsz, ysz);
	parent->add_child(bn);
	return bn;
}

}	// end of namespace utk
