// utk_scrollbar.cc

#include "utk_scrollbar.h"
#include "utk_gfx.h"

namespace utk {

IVec2 ScrollBar::get_cursor_tl() const
{
	IVec2 rel(cursor_pos + 2, 2);
	return rel + get_global_pos();
}
IVec2 ScrollBar::get_cursor_br() const
{
	IVec2 rel(cursor_pos + cursor_width, 2 + cursor_height);
	return rel + get_global_pos();
}

ScrollBar::ScrollBar(int cursor_width, utk::Callback cb)
{
	dragging = false;
	this->cursor_width = cursor_width;
	cursor_height = 20;
	cursor_pos = 0;
	set_size(101 + cursor_width + 4, cursor_height + 4);
	set_border(2);
	set_color(128, 100, 80);
}

ScrollBar::~ScrollBar()
{

}

Widget *ScrollBar::handle_event(Event *event)
{
	Widget *w;
	if(child && (w = child->handle_event(event))) {
		return w;
	}

	MButtonEvent *mb;
	if((mb = dynamic_cast<MButtonEvent*>(event)) && hit_test(mb->x, mb->y))
	{
		dragging = false;
		if (mb->pressed)
		{
			if (mb->button == MOUSE_LEFT)
			{
				if (rect_test(get_cursor_tl(), get_cursor_br(), IVec2(mb->x, mb->y)))
					dragging = true;
			}
		}
		return this;
	}

	// no child handled the event, either we do or return false
	MMotionEvent *mmev;
	if((mmev = dynamic_cast<MMotionEvent*>(event))) 
	{
		if (dragging)
		{
			int dx = mmev->x - get_last_drag_pos().x;
			if (dx < 0 && mmev->x > get_cursor_br().x) dx = 0;
			if (dx > 0 && mmev->x < get_cursor_tl().x) dx = 0;
			cursor_pos += dx;
			if (cursor_pos < 0) cursor_pos = 0;
			if (cursor_pos > 100) cursor_pos = 100;
			return this;
		}
	}

	return 0;
}

void ScrollBar::draw() const
{
	IVec2 gpos = get_global_pos();

	gfx::color(color.r, color.g, color.b, color.a);
	gfx::rect(gpos.x, gpos.y, gpos.x + size.x, gpos.y + size.y);

	// draw cursor line
	gfx::color(180, 0, 0, 255);
	gfx::rect(gpos.x + 2, gpos.y + size.y / 2 - 2, 
		gpos.x + size.x - 4, gpos.y + size.y / 2 + 2);

	// draw cursor
	gfx::color(0, 0, dragging ? 255 : 127, 255);
	IVec2 tl, br;
	tl = get_cursor_tl();
	br = get_cursor_br();
	gfx::rect(tl.x, tl.y, br.x, br.y);

	gfx::color(255, 255, 255, 255);
	gfx::circle(tl.x, tl.y, br.x, br.y, false);

	if(border) {
		gfx::color((int)(color.r * 1.25), (int)(color.g * 1.25), (int)(color.b * 1.25), color.a);

		gfx::line(gpos.x, gpos.y, gpos.x + size.x, gpos.y, border);
		gfx::line(gpos.x, gpos.y + size.y, gpos.x + size.x, gpos.y + size.y, border);
		gfx::line(gpos.x, gpos.y, gpos.x, gpos.y + size.y, border);
		gfx::line(gpos.x + size.x, gpos.y, gpos.x + size.x, gpos.y + size.y, border);
	}

	Widget::draw();
}

int ScrollBar::get_percent() const
{
	return cursor_pos;
}

}
