// utk_scrollbar.cc

#include "utk_scrollbar.h"
#include "utk_gfx.h"

namespace utk {

IVec2 Scrollbar::get_cursor_tl() const
{
	IVec2 rel((int)(val * track_len) + track_start - cursor_width / 2, border);
	return rel + get_global_pos();
}
IVec2 Scrollbar::get_cursor_br() const
{
	IVec2 rel((int)(val * track_len) + track_start + cursor_width / 2, size.y - border);
	return rel + get_global_pos();
}

Scrollbar::Scrollbar(utk::Callback cb)
{
	dragging = false;
	cursor_width = 20;
	val = 0;
	set_size(100 + border * 2, 20 + border * 2);
	set_border(2);
	set_color(128, 100, 80);
	set_callback(EVENT_MODIFY, cb);
	orient = HORIZONTAL;
}

Scrollbar::~Scrollbar() {}

Widget *Scrollbar::handle_event(Event *event)
{
	Widget *w;
	if(child && (w = child->handle_event(event))) {
		return w;
	}

	MButtonEvent *mb;
	if((mb = dynamic_cast<MButtonEvent*>(event)))
	{
		dragging = false;
		if (mb->pressed && hit_test(mb->x, mb->y))
		{
			if (mb->button == MOUSE_LEFT)
			{
				if (rect_test(get_cursor_tl(), get_cursor_br(), IVec2(mb->x, mb->y)))
					dragging = true;
			}
			return this;
		}
	}

	// no child handled the event, either we do or return false
	MMotionEvent *mmev;
	if((mmev = dynamic_cast<MMotionEvent*>(event))) {
		if(dragging) {
			int dx = mmev->x - get_last_drag_pos().x;
			if(dx < 0 && mmev->x > get_cursor_br().x) dx = 0;
			if(dx > 0 && mmev->x < get_cursor_tl().x) dx = 0;
			
			if(dx) {
				int cursor_pos = get_cursor_tl().x + dx - get_global_pos().x;
				val = (float)(cursor_pos - border) / (float)track_len;
				val = val < 0.0 ? 0.0 : (val > 1.0 ? 1.0 : val);

				// TODO: also call callback and on_modify
			}
			return this;
		}
	}

	return 0;
}

void Scrollbar::set_size(int w, int h)
{
	Widget::set_size(w, h);
	track_start = border + cursor_width / 2;
	track_len = w - border * 2 - cursor_width / 2;
}

void Scrollbar::set_value(float val)
{
	this->val = val;
	// TODO: modify event
}

float Scrollbar::get_value() const
{
	return val;
}

void Scrollbar::operator=(float val)
{
	this->val = val;
	// TODO: modify event
}

Scrollbar::operator float() const
{
	return val;
}

void Scrollbar::draw() const
{
	IVec2 gpos = get_global_pos();

	gfx::color(color.r, color.g, color.b, color.a);
	gfx::rect(gpos.x, gpos.y, gpos.x + size.x, gpos.y + size.y);

	// draw cursor line
	gfx::color(127, 127, 127, 255);
	gfx::rect(gpos.x + 2, gpos.y + size.y / 2 - 2, 
		gpos.x + size.x - 4, gpos.y + size.y / 2 + 2);

	// draw cursor
	gfx::color(0, 0, dragging ? 255 : 127, 255);
	IVec2 tl, br;
	tl = get_cursor_tl();
	br = get_cursor_br();
	//gfx::rect(tl.x, tl.y, br.x, br.y);

	gfx::color(127, 127, 127, 255);
	gfx::circle(tl.x, tl.y, br.x, br.y, false);
	if (dragging)
	{
		gfx::color(0, 255, 0, 255);
	}
	else
	{
		gfx::color(50, 50, 50, 255);
	}
	gfx::circle(tl.x + 3, tl.y + 3, br.x - 3, br.y - 3, false);

	if(border) {
		gfx::color((int)(color.r * 1.25), (int)(color.g * 1.25), (int)(color.b * 1.25), color.a);

		gfx::line(gpos.x, gpos.y, gpos.x + size.x, gpos.y, border);
		gfx::line(gpos.x, gpos.y + size.y, gpos.x + size.x, gpos.y + size.y, border);
		gfx::line(gpos.x, gpos.y, gpos.x, gpos.y + size.y, border);
		gfx::line(gpos.x + size.x, gpos.y, gpos.x + size.x, gpos.y + size.y, border);
	}

	Widget::draw();
}


}
