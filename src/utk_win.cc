#include "utk_win.h"
#include "utk_gfx.h"
#include "utk_events.h"

#include <stdio.h>

namespace utk {

Window::Window()
{
	visible = false;
	color.r = color.g = color.b = 128;
	border = 2;
	padding = 4;
	tbar_height = 20;
}

Window::~Window() {}

Widget *Window::handle_event(Event *event)
{
	Widget *w;
	if(child && (w = child->handle_event(event))) {
		return w;
	}

	// no child handled the event, either we do or return false
	MMotionEvent *mmev;
	if((mmev = dynamic_cast<MMotionEvent*>(event))) {
		if(hit_test(mmev->x, mmev->y) && get_button_state() == MOUSE_LEFT && get_button_press_widget() == this) {
			int dx = mmev->x - get_last_drag_pos().x;
			int dy = mmev->y - get_last_drag_pos().y;

			pos.x += dx;
			pos.y += dy;
			return this;
		}
	}

	MButtonEvent *bev;
	if((bev = dynamic_cast<MButtonEvent*>(event)) && hit_test(bev->x, bev->y)) {
		return this;
	}

	return 0;
}

bool Window::hit_test(int x, int y) const
{
	IVec2 gpos = get_global_pos();

	int top = text.size() ? gpos.y - tbar_height : gpos.y;
	return visible && x >= gpos.x && x < gpos.x + size.x && y >= top && y < gpos.y + size.y;
}

void Window::draw() const
{
	IVec2 gpos = get_global_pos();

	gfx::color(color.r, color.g, color.b, color.a);
	gfx::rect(gpos.x, gpos.y, gpos.x + size.x, gpos.y + size.y);

	/* draw titlebar and borders if appropriate */
	if(!border && !text.size()) {
		Widget::draw();
		return;
	}

	gfx::color((int)(color.r * 1.25), (int)(color.g * 1.25), (int)(color.b * 1.25), color.a);

	int sy = text.size() ? gpos.y - tbar_height : gpos.y;
	
	gfx::line(gpos.x, sy, gpos.x + size.x, sy, border);
	gfx::line(gpos.x, gpos.y + size.y, gpos.x + size.x, gpos.y + size.y, border);
	gfx::line(gpos.x, sy, gpos.x, gpos.y + size.y, border);
	gfx::line(gpos.x + size.x, sy, gpos.x + size.x, gpos.y + size.y, border);

	if(text.size()) {
		gfx::rect(gpos.x, gpos.y - tbar_height, gpos.x + size.x, gpos.y);

		gfx::color(0, 0, 0, color.a);
		gfx::text(gpos.x + 2, gpos.y, text.c_str(), 18);
	}

	Widget::draw();
}

}	// namespace utk end
