#include "utk_scrollwin.h"
#include "utk_gfx.h"
#include "utk_common.h"

namespace utk {

ScrollWin::ScrollWin()
{
	set_color(100, 100, 100);
	set_size(64, 64);
}

ScrollWin::~ScrollWin() {}

Widget *ScrollWin::handle_event(Event *event)
{
	Widget *w;
	if(child) {
		/*if((w = child->handle_event(event))) {
			return w;
		}*/

		MMotionEvent *mmev;
		if((mmev = dynamic_cast<MMotionEvent*>(event))) {
			if(hit_test(mmev->x, mmev->y) && get_button_state() == MOUSE_LEFT && get_button_press_widget() == this) {
				int dx = mmev->x - get_last_drag_pos().x;
				int dy = mmev->y - get_last_drag_pos().y;

				IVec2 cpos = child->get_pos();
				IVec2 csz = child->get_size();
				
				int max_x = MAX(csz.x - size.x, 0);
				int max_y = MAX(csz.y - size.y, 0);

				int new_x = CLAMP(cpos.x + dx, -max_x, 0);
				int new_y = CLAMP(cpos.y + dy, -max_y, 0);

				child->set_pos(new_x, new_y);
				return this;
			}
		}

		MButtonEvent *bev;
		if((bev = dynamic_cast<MButtonEvent*>(event)) && hit_test(bev->x, bev->y)) {
			return this;
		}
	}

	return 0;
}

void ScrollWin::draw() const
{
	IVec2 gpos = get_global_pos();

	gfx::color(color.r, color.g, color.b, color.a);
	gfx::rect(gpos.x, gpos.y, gpos.x + size.x, gpos.y + size.y);

	Widget::draw();
}

}	// end utk namespace
