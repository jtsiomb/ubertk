#include "utk_slider.h"
#include "utk_gfx.h"

namespace utk {

void Slider::initialize()
{
	start = end = 0;
	vis_decimal = 2;
	show_value = true;
	link_float = 0;
	link_int = 0;
}

Slider::Slider(float start, float end, utk::Callback cb)
	: ScrollBar(8)
{
	initialize();
	this->start = start;
	this->end = end;
	set_callback(EVENT_MODIFY, cb);
}

/*
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
*/

Slider::~Slider() {}

#define NORM_TO_PERC(x)	(int)(100.0 * (x))
#define PERC_TO_NORM(x)	((float)(x) / 100.0)

void Slider::set_value(float val)
{
	cursor_pos = NORM_TO_PERC((val - start) / (end - start));
}

float Slider::get_value() const
{
	return PERC_TO_NORM(cursor_pos) * (end - start) + start;
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
	Widget *ret = ScrollBar::handle_event(event);

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

	gfx::color(color.r * 1.5, color.g * 1.5, color.b * 1.5, color.a);
	IVec2 tl = get_cursor_tl();
	IVec2 br = get_cursor_br();
	gfx::rect(tl.x, tl.y, br.x, br.y);

	gfx::color(color.r * 2.0, color.g * 1.5, color.b, color.a);
	gfx::rect(gpos.x + border, gpos.y + border, tl.x, gpos.y + size.y - border);

	if(border) {
		gfx::color((int)(color.r * 1.25), (int)(color.g * 1.25), (int)(color.b * 1.25), color.a);

		gfx::line(gpos.x, gpos.y, gpos.x + size.x, gpos.y, border);
		gfx::line(gpos.x, gpos.y + size.y, gpos.x + size.x, gpos.y + size.y, border);
		gfx::line(gpos.x, gpos.y, gpos.x, gpos.y + size.y, border);
		gfx::line(gpos.x + size.x, gpos.y, gpos.x + size.x, gpos.y + size.y, border);
	}

	Widget::draw();
}

}	// end namespace utk
