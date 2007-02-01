#include "utk_slider.h"
#include "utk_gfx.h"

namespace utk {

void Slider::initialize()
{
	cursor_width = 10;
	start = end = 0;
	vis_decimal = 2;
	show_value = true;
	link_float = 0;
	link_int = 0;
}

Slider::Slider(float start, float end, utk::Callback cb)
{
	initialize();
	this->start = start;
	this->end = end;
	set_callback(EVENT_MODIFY, cb);
}

Slider::Slider(float start, float end, float *link)
{
	initialize();
	this->start = start;
	this->end = end;
	link_float = link;
}

Slider::Slider(float start, float end, int *link)
{
	initialize();
	this->start = start;
	this->end = end;
	link_int = link;
}

Slider::~Slider() {}

#define NORM_TO_PERC(x)	(int)(100.0 * (x))
#define PERC_TO_NORM(x)	((float)(x) / 100.0)

void Slider::set_value(float val)
{
	val = (val - start) / (end - start);
}

float Slider::get_value() const
{
	return val * (end - start) + start;
}

void Slider::operator=(float val)
{
	set_value(val);
}

Slider::operator float() const
{
	return get_value();
}

Widget *Slider::handle_event(Event *event)
{
	Widget *ret = Scrollbar::handle_event(event);

	if(link_float) {
		*link_float = get_value();
	}
	if(link_int) {
		*link_int = (int)get_value();
	}

	return ret;
}

void Slider::draw() const
{
	IVec2 gpos = get_global_pos();

	gfx::color(color.r, color.g, color.b, color.a);
	gfx::rect(gpos.x, gpos.y, gpos.x + size.x, gpos.y + size.y);

	if(dragging) {
		gfx::color_clamp((int)(color.r * 1.5), (int)(color.g * 1.5), (int)(color.b * 1.5), color.a);
	} else {
		gfx::color_clamp((int)(color.r * 1.4), (int)(color.g * 1.4), (int)(color.b * 1.4), color.a);
	}
	IVec2 tl = get_cursor_tl();
	IVec2 br = get_cursor_br();
	gfx::rect(tl.x, tl.y, br.x, br.y);

	gfx::color_clamp((int)(color.r * 1.1), (int)(color.g * 1.5), (int)(color.b * 1.6), color.a);
	gfx::rect(gpos.x + border, gpos.y + border, tl.x, gpos.y + size.y - border);

	if(border) {
		gfx::color_clamp((int)(color.r * 1.25), (int)(color.g * 1.25), (int)(color.b * 1.25), color.a);

		gfx::line(gpos.x, gpos.y, gpos.x + size.x, gpos.y, border);
		gfx::line(gpos.x, gpos.y + size.y, gpos.x + size.x, gpos.y + size.y, border);
		gfx::line(gpos.x, gpos.y, gpos.x, gpos.y + size.y, border);
		gfx::line(gpos.x + size.x, gpos.y, gpos.x + size.x, gpos.y + size.y, border);
	}

	if(show_value) {
		char txbuf[128];
		char fmt[8];

		sprintf(fmt, vis_decimal ? "%%.%df" : "%%d", vis_decimal);
		sprintf(txbuf, fmt, vis_decimal ? get_value() : (int)get_value());

		gfx::color(0, 0, 0, color.a);
		gfx::text(gpos.x + border, gpos.y + size.y - border, txbuf, 18);
	}

	Widget::draw();
}

}	// end namespace utk
