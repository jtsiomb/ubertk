#include "utk_win.h"
#include "utk_gfx.h"
#include "utk_events.h"
#include "utk_common.h"

#include <stdio.h>

namespace utk {

unsigned int get_msec();

Window::Window()
{
	visible = false;
	focused = false;
	set_color(128, 128, 128, 255);
	border = 3;
	padding = 4;
	tbar_height = 20;
}

Window::~Window() {}

Widget *Window::handle_event(Event *event)
{
	FocusEvent *fev;
	if((fev = dynamic_cast<FocusEvent*>(event))) {
		focused = fev->focus;
		callback(fev, EVENT_FOCUS);
		return this;
	}

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

void Window::rise()
{
	if(parent && dynamic_cast<WinFrame*>(parent)) {
		parent->rise();
	} else {
		Widget::rise();
	}
}

void Window::sink()
{
	if(parent && dynamic_cast<WinFrame*>(parent)) {
		parent->sink();
	} else {
		Widget::sink();
	}
}

void Window::set_win_focus(Widget *w)
{
	win_focus = w;
}

Widget *Window::get_win_focus()
{
	return win_focus;
}

const Widget *Window::get_win_focus() const
{
	return win_focus;
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
	unfocused_col = Color(100, 110, 130);
	shaded = false;

	if((win = dynamic_cast<Window*>(child))) {
		add_child(child);
		update_geometry();

		int cbord = win->get_border();
		set_pos(win->get_pos() - IVec2(cbord, cbord + win->tbar_height));
		win->set_pos(cbord, cbord + win->tbar_height);
	} else {
		utk_error("non-window widget passed as WinFrame's child\n");
	}

	last_click = 0;
}

WinFrame::~WinFrame() {}

void WinFrame::update_geometry()
{
	Window *win = (Window*)this->child;
	
	int cbord = win->get_border();

	IVec2 csz = win->get_size();
	set_size(csz.x + cbord * 2, csz.y + cbord * 2 + win->tbar_height);
}

void WinFrame::set_shade(bool shade)
{
	shaded = shade;

	if(shade) {
		Window *cwin = (Window*)child;
		orig_size = size;
		set_size(orig_size.x, cwin->get_border() * 2 + cwin->tbar_height);
	} else {
		set_size(orig_size.x, orig_size.y);
	}
}

Widget *WinFrame::handle_event(Event *event)
{
	Widget *w;
	if(!shaded && child && (w = child->handle_event(event))) {
		return w;
	}


	// no child handled the event, either we do or return false
	MMotionEvent *mmev;
	if((mmev = dynamic_cast<MMotionEvent*>(event))) {
		if(/*hit_test(mmev->x, mmev->y) &&*/ get_button_state() == MOUSE_LEFT && get_button_press_widget() == this) {
		
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

			Window *cwin = (Window*)child;
			IVec2 gpos = get_global_pos();
			int bord = cwin->get_border();

			if(cev->x >= gpos.x + bord && cev->x < gpos.x + size.x - bord &&
					cev->y >= gpos.y + bord && cev->y < gpos.y + cwin->tbar_height + bord) {
				unsigned int msec = get_msec();
				if(msec - last_click < 400) {
					set_shade(!shaded);
				} else {
					last_click = msec;
				}
			}
			
			return this;
		}
	}
	
	return 0;
}

bool WinFrame::is_visible() const
{
	return child->is_visible();
}

void WinFrame::draw() const
{
	Window *child = (Window*)this->child;

	IVec2 gpos = get_global_pos();
	int bord = child->get_border();
	int third = bord / 3;

	int alpha = child->get_color().a;

	if(child->focused) {
		gfx::color_clamp(color.r, color.g, color.b, alpha);
	} else {
		gfx::color_clamp(unfocused_col.r, unfocused_col.g, unfocused_col.b, alpha);
	}

	Rect out(gpos.x, gpos.y, gpos.x + size.x, gpos.y + size.y);
	Rect in(gpos.x + bord - third, gpos.y + bord - third, gpos.x + size.x - bord + third, gpos.y + size.y - bord + third);

	if(bord < 3) {
		gfx::bevel(out.x1, out.y1, out.x2, out.y2, 0, bord);
	} else {
		gfx::frame(out.x1, out.y1, out.x2, out.y2, bord);
		gfx::bevel(out.x1, out.y1, out.x2, out.y2, 0, third);
		gfx::bevel(in.x1, in.y1, in.x2, in.y2, gfx::BEVEL_INSET, third);
	}

	bevel(gpos.x + bord, gpos.y + bord, gpos.x + size.x - bord, gpos.y + bord + child->tbar_height, gfx::BEVEL_FILLBG, MAX(1, third));

	const char *txt = child->get_text();
	if(txt) {
		int twidth = gfx::text_width(txt, 18);

		gfx::color(0, 0, 0, color.a);
		gfx::text(gpos.x + (size.x - twidth) / 2, gpos.y + bord + child->tbar_height - 1, txt, 18);
	}

	if(!shaded) {
		Widget::draw();
	}
}


Window *create_window(Widget *parent, int x, int y, int w, int h, const char *title)
{
	Window *win = new Window;
	win->set_pos(x, y);
	win->set_size(w, h);
	win->set_text(title);

	WinFrame *frm = new WinFrame(win);
	parent->add_child(frm);

	win->rise();
	return win;
}

void destroy_window(Widget *w)
{
	if(dynamic_cast<Window*>(w)) {
		/*
		Widget *parent = w->get_parent();
		
		if(dynamic_cast<WinFrame*>(parent)) {
			w = parent;
			parent = w->get_parent();
		}

		if(parent) {
			parent->remove_child(w);
		}
		*/
		utk::destruct_queue.push_back(w);
	} else {
		utk_error("non-window widget passed to destroy_window()\n");
	}
}

}	// namespace utk end
