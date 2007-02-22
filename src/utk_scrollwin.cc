#include "utk_scrollwin.h"
#include "utk_gfx.h"
#include "utk_common.h"

namespace utk {

static void vbar_handler(Event *ev, void *data);
static void hbar_handler(Event *ev, void *data);

ScrollWin::ScrollWin()
{
	vbar = new Scrollbar(VERTICAL);
	hbar = new Scrollbar(HORIZONTAL);
	vbar->set_callback(EVENT_MODIFY, vbar_handler, this);
	hbar->set_callback(EVENT_MODIFY, hbar_handler, this);
	vbar->set_parent(this);	// to get positioning right
	hbar->set_parent(this);	// ditto
	vbar->set_border(0);
	hbar->set_border(0);
	vbar_state = SCROLLBAR_AUTO;
	hbar_state = SCROLLBAR_AUTO;

	sbar_width = 15;
	
	set_color(100, 100, 100);
	set_size(64, 64);
	
}

ScrollWin::~ScrollWin()
{
	delete vbar;
	delete hbar;
}

bool ScrollWin::check_hscroll_vis() const
{
	IVec2 child_size = child->get_size();
	return hbar_state == SCROLLBAR_SHOW || (hbar_state == SCROLLBAR_AUTO && child_size.x > size.x);
}

bool ScrollWin::check_vscroll_vis() const
{
	IVec2 child_size = child->get_size();
	return vbar_state == SCROLLBAR_SHOW || (vbar_state == SCROLLBAR_AUTO && child_size.y > size.y);
}

Widget *ScrollWin::handle_event(Event *event)
{
	if(child) {
		/*Widget *w;
		if((w = child->handle_event(event))) {
			return w;
		}*/

		if(hbar->handle_event(event)) {
			return hbar;
		}
		if(vbar->handle_event(event)) {
			return vbar;
		}

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

				hbar->set_value(get_hscroll());
				vbar->set_value(get_vscroll());
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

void ScrollWin::set_size(int w, int h)
{
	Widget::set_size(w, h);
	
	hbar->set_size(size.x, sbar_width);
	hbar->set_pos(0, size.y - sbar_width);

	vbar->set_size(sbar_width, size.y);
	vbar->set_pos(size.x - sbar_width, 0);
}

void ScrollWin::set_size(IVec2 sz)
{
	set_size(sz.x, sz.y);
}

void ScrollWin::set_scrollbar_state(unsigned int which, int state)
{
	if(which & SCROLLBAR_VERTICAL) {
		vbar_state = state;
	}
	if(which & SCROLLBAR_HORIZONTAL) {
		hbar_state = state;
	}
}

int ScrollWin::get_scrollbar_state(unsigned int which)
{
	if(which == SCROLLBAR_VERTICAL) {
		return vbar_state;
	}
	if(which == SCROLLBAR_HORIZONTAL) {
		return hbar_state;
	}
	return -1;
}


void ScrollWin::set_hscroll(float val)
{
	int child_xsz = child->get_size().x;
	int max_x = MAX(child_xsz - size.x, 0);

	if(check_vscroll_vis()) {
		max_x += sbar_width;
	}

	child->set_pos(-(int)(max_x * val), child->get_pos().y);
}

void ScrollWin::set_vscroll(float val)
{
	int child_ysz = child->get_size().y;
	int max_y = MAX(child_ysz - size.y, 0);

	if(check_hscroll_vis()) {
		max_y += sbar_width;
	}

	child->set_pos(child->get_pos().x, -(int)(max_y * val));
}

float ScrollWin::get_hscroll() const
{
	int child_xsz = child->get_size().x;
	int max_x = MAX(child_xsz - size.x, 0);
	return -(float)child->get_pos().x / (float)max_x;
}

float ScrollWin::get_vscroll() const
{
	int child_ysz = child->get_size().y;
	int max_y = MAX(child_ysz - size.y, 0);
	return -(float)child->get_pos().y / (float)max_y;
}


void ScrollWin::draw() const
{
	IVec2 gpos = get_global_pos();

	gfx::color(color.r, color.g, color.b, color.a);
	gfx::rect(gpos.x, gpos.y, gpos.x + size.x, gpos.y + size.y);

	Widget::draw();

	IVec2 child_size = child->get_size();

	bool show_hbar = check_hscroll_vis();
	bool show_vbar = check_vscroll_vis();

	if(show_hbar) {
		int hbar_sz = show_vbar ? size.x - vbar->get_size().x : size.x;
		hbar->set_size(hbar_sz, hbar->get_size().y);

		float width_ratio = (float)size.x / (float)child_size.x;
		hbar->set_cursor_width(width_ratio); 
		hbar->draw();
	}
	
	if(show_vbar) {
		int vbar_sz = show_hbar ? size.y - hbar->get_size().y : size.y;
		vbar->set_size(vbar->get_size().x, vbar_sz);

		float width_ratio = (float)size.y / (float)child_size.y;
		vbar->set_cursor_width(width_ratio); 
		vbar->draw();
	}

	if(show_vbar && show_hbar) {
		Color col = darker_color(vbar->get_color());
		
		gfx::color(col.r, col.g, col.b, col.a);
		gfx::rect(gpos.x + size.x - sbar_width, gpos.y + size.y - sbar_width, gpos.x + size.x, gpos.y + size.y);
	}
}

static void vbar_handler(Event *ev, void *data)
{
	ScrollWin *swin;
	if(!(swin = dynamic_cast<ScrollWin*>((Widget*)data))) {
		return;
	}

	Scrollbar *sbar;
	if(ev->widget && (sbar = dynamic_cast<Scrollbar*>(ev->widget))) {
		swin->set_vscroll(sbar->get_value());
	}
}

static void hbar_handler(Event *ev, void *data)
{
	ScrollWin *swin;
	if(!(swin = dynamic_cast<ScrollWin*>((Widget*)data))) {
		return;
	}

	Scrollbar *sbar;
	if(ev->widget && (sbar = dynamic_cast<Scrollbar*>(ev->widget))) {
		swin->set_hscroll(sbar->get_value());
	}
}

}	// end utk namespace
