// utk_scrollbar.cc

#include "utk_scrollbar.h"
#include "utk_gfx.h"

namespace utk {

IVec2 ScrollBar::GetCursorTL() const
{
	IVec2 rel(cursor_pos + 2, 2);
	return rel + get_global_pos();
}
IVec2 ScrollBar::GetCursorBR() const
{
	IVec2 rel(cursor_pos + cursor_width, 2 + cursor_height);
	return rel + get_global_pos();
}

ScrollBar::ScrollBar(int cursor_width, utk::Callback cb)
{
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

bool ScrollBar::handle_event(Event *event)
{
	if(child && child->handle_event(event)) {
		return true;
	}

	// no child handled the event, either we do or return false
	MMotionEvent *mmev;
	if((mmev = dynamic_cast<MMotionEvent*>(event))) {
		if(get_button_state() == MOUSE_LEFT) {
			if (RectTest(GetCursorTL(), GetCursorBR(), IVec2(mmev->x, mmev->y)))
			{
				int dx = mmev->x - get_last_drag_pos().x;
				cursor_pos += dx;
				if (cursor_pos > 100) cursor_pos = 100;
				if (cursor_pos < 0) cursor_pos = 0;
				return true;
			}
		}
	}

	return false;
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
	gfx::color(0, 0, 255, 255);
	IVec2 tl, br;
	tl = GetCursorTL();
	br = GetCursorBR();
	gfx::rect(tl.x, tl.y, br.x, br.y);

	if(border) {
		gfx::color((int)(color.r * 1.25), (int)(color.g * 1.25), (int)(color.b * 1.25), color.a);

		gfx::line(gpos.x, gpos.y, gpos.x + size.x, gpos.y);
		gfx::line(gpos.x, gpos.y + size.y, gpos.x + size.x, gpos.y + size.y);
		gfx::line(gpos.x, gpos.y, gpos.x, gpos.y + size.y);
		gfx::line(gpos.x + size.x, gpos.y, gpos.x + size.x, gpos.y + size.y);
	}

	Widget::draw();
}

int ScrollBar::GetPercent()
{
	return cursor_pos;
}

}