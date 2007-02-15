#include "utk_scrollwin.h"
#include "utk_gfx.h"
#include "utk_macros.h"

namespace utk {

ScrollWin::ScrollWin()
{
	set_color(100, 100, 100);
	view_pos.x = view_pos.y = 0;
	view_size = size;
}

ScrollWin::~ScrollWin() {}

Widget *ScrollWin::handle_event(Event *event)
{
	Widget *w;
	if(child && (w = child->handle_event(event))) {
		return w;
	}

	MMotionEvent *mmev;
	if((mmev = dynamic_cast<MMotionEvent*>(event))) {
		if(hit_test(mmev->x, mmev->y) && get_button_state() == MOUSE_LEFT && get_button_press_widget() == this) {
			int dx = mmev->x - get_last_drag_pos().x;
			int dy = mmev->y - get_last_drag_pos().y;

			int new_x = view_pos.x + dx;
			int new_y = view_pos.y - dy;

			int max_x = MAX(size.x - view_size.x, 0);
			int max_y = MAX(size.y - view_size.y, 0);

			view_pos.x = CLAMP(new_x, 0, max_x);
			view_pos.y = CLAMP(new_y, 0, max_y);
			return this;
		}
	}

	MButtonEvent *bev;
	if((bev = dynamic_cast<MButtonEvent*>(event)) && hit_test(bev->x, bev->y)) {
		return this;
	}

	return 0;
}

IVec2 ScrollWin::get_global_pos() const
{
	IVec2 fake_pos = pos - view_pos;
	return parent ? fake_pos + parent->get_global_pos() : fake_pos;
}

void ScrollWin::set_view_pos(int x, int y)
{
	view_pos.x = x;
	view_pos.y = y;
}

void ScrollWin::set_view_pos(IVec2 pos)
{
	view_pos = pos;
}

IVec2 ScrollWin::get_view_pos() const
{
	return view_pos;
}

void ScrollWin::set_view_size(int w, int h)
{
	view_size.x = w;
	view_size.y = h;
}

void ScrollWin::set_view_size(IVec2 sz)
{
	view_size = sz;
}

IVec2 ScrollWin::get_view_size() const
{
	return view_size;
}

int ScrollWin::get_view_width() const
{
	return view_size.x;
}

int ScrollWin::get_view_height() const
{
	return view_size.y;
}

void ScrollWin::draw() const
{
	// NOTE: don't use the get_global_pos() here... it lies! (to make child positioning work)
	IVec2 gpos = parent ? pos + parent->get_global_pos() : pos;

	gfx::color(color.r, color.g, color.b, color.a);
	gfx::rect(gpos.x, gpos.y, gpos.x + view_size.x, gpos.y + view_size.y);

	Widget::draw();
}

}	// end utk namespace
