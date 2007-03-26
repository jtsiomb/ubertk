#include "utk_slider.h"
#include "utk_gfx.h"

namespace utk {

Slider::Slider()
{
	cursor_width = 10;
	start = end = 0;
	vis_decimal = 2;
	show_value = true;

	set_size(size.x, size.y);
}

Slider::~Slider() {}

void Slider::set_min(float start)
{
	this->start = start;
}

void Slider::set_max(float end)
{
	this->end = end;
}

void Slider::set_range(float start, float end)
{
	this->start = start;
	this->end = end;
}

float Slider::get_min() const
{
	return start;
}

float Slider::get_max() const
{
	return end;
}

void Slider::set_vis_decimal(int count)
{
	vis_decimal = count;
}

int Slider::get_vis_decimal() const
{
	return vis_decimal;
}


void Slider::set_value(float val)
{
	Scrollbar::set_value((val - start) / (end - start));
}

float Slider::get_value() const
{
	return Scrollbar::get_value() * (end - start) + start;
}

void Slider::operator=(float val)
{
	set_value(val);
}

Slider::operator float() const
{
	return get_value();
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

void Slider::on_modify(Event *event)
{
	float val = get_value();

	if(link_flt) {
		*link_flt = val;
	}
	if(link_int) {
		*link_int = (int)val;
	}
	if(link_str) {
		char fmt[8];
		sprintf(fmt, vis_decimal ? "%%.%df" : "%%d", vis_decimal);
		#if (defined(WIN32) && !defined(_GNUC_))	// assume msvc
		sprintf(link_str, fmt, val);
		#else
		snprintf(link_str, link_str_width, fmt, val);
		#endif
	}
}

Slider *create_slider(Widget *parent, float start, float end, Callback cb, void *cdata)
{
	Slider *slider = new Slider;
	slider->set_range(start, end);
	slider->set_callback(EVENT_MODIFY, cb, cdata);
	parent->add_child(slider);
	return slider;
}

Slider *create_slider(Widget *parent, float start, float end, float *link)
{
	Slider *slider = create_slider(parent, start, end);
	slider->set_link(link);
	return slider;
}

Slider *create_slider(Widget *parent, float start, float end, int *link)
{
	Slider *slider = create_slider(parent, start, end);
	slider->set_link(link);
	return slider;
}

Slider *create_slider(Widget *parent, float start, float end, char *link, int bufsz)
{
	Slider *slider = create_slider(parent, start, end);
	slider->set_link(link, bufsz);
	return slider;
}


}	// end namespace utk
