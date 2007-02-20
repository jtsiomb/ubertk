#include "utk_win.h"
#include "utk_gfx.h"
#include "utk_events.h"
#include "utk_common.h"

#include <stdio.h>

namespace utk {

Window::Window()
{
	visible = false;
	set_color(128, 128, 128, 255);
	border = 3;
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
	return 0;
}

void Window::set_size(int w, int h)
{
	WinFrame *frm;

	Widget::set_size(w, h);
	if((frm = dynamic_cast<WinFrame*>(parent))) {
		frm->update_geometry();
	}
}

void Window::set_size(IVec2 sz)
{
	set_size(sz.x, sz.y);
}

void Window::draw() const
{
	IVec2 gpos = get_global_pos();

	gfx::color(color.r, color.g, color.b, color.a);
	gfx::rect(gpos.x, gpos.y, gpos.x + size.x, gpos.y + size.y);

	Widget::draw();
}


// ------- WinFrame -------

WinFrame::WinFrame(Widget *child)
{
	Window *win;

	set_color(128, 130, 200);

	if((win = dynamic_cast<Window*>(child))) {
		add_child(child);
		update_geometry();

		int cbord = win->get_border();
		set_pos(win->get_pos() - IVec2(cbord, cbord + win->tbar_height));
		win->set_pos(cbord, cbord + win->tbar_height);
	} else {
		utk_error("non-window widget passed as WinFrame's child\n");
	}
}

WinFrame::~WinFrame() {}

void WinFrame::update_geometry()
{
	Window *win = (Window*)this->child;
	
	int cbord = win->get_border();

	IVec2 csz = win->get_size();
	set_size(csz.x + cbord * 2, csz.y + cbord * 2 + win->tbar_height);
}

Widget *WinFrame::handle_event(Event *event)
{
	if(child && child->handle_event(event)) {
		return child;
	}

	// no child handled the event, either we do or return false
	MMotionEvent *mmev;
	if((mmev = dynamic_cast<MMotionEvent*>(event))) {
		if(hit_test(mmev->x, mmev->y) && get_button_state() == MOUSE_LEFT && get_button_press_widget() == this) {
			int dx = mmev->x - get_last_drag_pos().x;
			int dy = mmev->y - get_last_drag_pos().y;

			set_pos(pos.x + dx, pos.y + dy);
			return this;
		}
	}

	MButtonEvent *bev;
	if((bev = dynamic_cast<MButtonEvent*>(event)) && hit_test(bev->x, bev->y)) {
		return this;
	}

	ClickEvent *cev;
	if((cev = dynamic_cast<ClickEvent*>(event))) {
		if(!cev->widget && hit_test(cev->x, cev->y)) {
			cev->widget = this;
			rise();
			return this;
		}
	}
	
	return 0;
}


void WinFrame::draw() const
{
	Window *child = (Window*)this->child;
	
	IVec2 gpos = get_global_pos();
	int bord = child->get_border();
	int third = bord / 3;

	gfx::color_clamp(color.r, color.g, color.b, color.a);

	Rect out(gpos.x, gpos.y, gpos.x + size.x, gpos.y + size.y);
	Rect in(gpos.x + bord - third, gpos.y + bord - third, gpos.x + size.x - bord + third, gpos.y + size.y - bord + third);

	if(bord < 3) {
		gfx::bevel(out.x1, out.y1, out.x2, out.y2, 0, bord);
	} else {
		bevel(out.x1, out.y1, out.x2, out.y2, gfx::BEVEL_FILLBG, third);
		gfx::bevel(in.x1, in.y1, in.x2, in.y2, gfx::BEVEL_FILLBG | gfx::BEVEL_INSET, third);
	}

	bevel(gpos.x + bord, gpos.y + bord, gpos.x + size.x - bord, gpos.y + bord + child->tbar_height, gfx::BEVEL_FILLBG, MAX(1, third));

	Widget::draw();
}


Window *create_window(Widget *parent, int x, int y, int w, int h, const char *title)
{
	Window *win = new Window;
	win->set_pos(x, y);
	win->set_size(w, h);
	win->set_text(title);

	WinFrame *frm = new WinFrame(win);
	parent->add_child(frm);
	return win;
}

}	// namespace utk end
