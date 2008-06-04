/*
ubertk is a flexible GUI toolkit targetted towards graphics applications.
Copyright (C) 2007 - 2008 John Tsiombikas <nuclear@member.fsf.org>,
                          Michael Georgoulopoulos <mgeorgoulopoulos@gmail.com>,
				          Kostas Michalopoulos <badsector@slashstone.com>

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
3. The name of the author may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/
#include <list>
#include <stack>
#include "ubertk.h"
#include "utk_events.h"

namespace utk {

static int focus_follows_mouse = 1;
static int mouse_button_state = -1;
static int last_press_x, last_press_y;
static Widget *mouse_press_widget;
static Widget *mouse_grab_widget;
static Widget *focused_window;
static Widget *last_hover_widget;
static IVec2 last_drag;
static IVec2 mouse_pos;
static std::stack<Window*> modal_windows;
static Window *current_modal_window;

static bool lock_destruct_queue;
std::list<Widget*> destruct_queue;

static void handle_event(Event *e);

Event::Event()
{
	event_id = EVENT_NULL;
	widget = 0;
}

Event::~Event() {}

int Event::get_event_id() const
{
	return event_id;
}

MouseEvent::MouseEvent(int x, int y)
{
	this->x = x;
	this->y = y;
}

MMotionEvent::MMotionEvent(int x, int y)
	: MouseEvent(x, y)
{
	event_id = EVENT_MMOTION;
}


MButtonEvent::MButtonEvent(int bn, int x, int y)
	: MouseEvent(x, y)
{
	event_id = EVENT_MBUTTON;
	button = bn;
}

MHoverEvent::MHoverEvent(bool enter, Widget *other, int x, int y)
	: MouseEvent(x, y)
{
	event_id = EVENT_MHOVER;
	this->enter = enter;
	this->other = other;
}

KeyboardEvent::KeyboardEvent(int key)
{
	event_id = EVENT_KEYBOARD;
	this->key = key;
}

// --- high level events ---
ClickEvent::ClickEvent()
{
	button = x = y = 0;
	time = 0;
}

ClickEvent::~ClickEvent() {}

FocusEvent::FocusEvent()
{
	focus = false;
}

FocusEvent::~FocusEvent() {}


// ------ low level event handling ------

void event(Event *e)
{
	handle_event(e);
	
	if(!destruct_queue.empty()) {
		lock_destruct_queue = true;
		std::list<Widget*>::iterator iter = destruct_queue.begin();
		while(iter != destruct_queue.end()) {
			Widget *w = *iter++;
			if(dynamic_cast<Window*>(w)) {
				Widget *parent = w->get_parent();
		
				if(dynamic_cast<WinFrame*>(parent)) {
					w = parent;
					parent = w->get_parent();
				}

				if(parent) {
					parent->remove_child(w);
				}
			}
		}
		lock_destruct_queue = false;
		
		destruct_queue.clear();
	}
}

static Widget *deliver_event(Widget *w, Event *e)
{
	if (!e || !w)
		return NULL;
	for (e->widget=NULL; w && !e->widget; w=w->get_parent())
		w->handle_event(e);
	return w;
}

static void handle_event(Event *e)
{
	Widget *root = get_root_widget();
	Widget *receiver;

	KeyboardEvent *kev;
	if((kev = dynamic_cast<KeyboardEvent*>(e))) {
		if(focused_window) {
			focused_window->handle_event(e);
		}
		return;
	}
	
	MMotionEvent *mev;
	if((mev = dynamic_cast<MMotionEvent*>(e))) {
		Widget *widget_under_mouse = root->get_child_at(mev->x, mev->y);
		receiver = mouse_grab_widget?mouse_grab_widget:widget_under_mouse;
		
		mouse_pos.x = mev->x;
		mouse_pos.y = mev->y;

		if (current_modal_window && receiver->get_window() != current_modal_window)
			return;

		deliver_event(receiver, mev);

		if (widget_under_mouse != last_hover_widget) {
			MHoverEvent hev(true, last_hover_widget, mev->x, mev->y);
			if (widget_under_mouse) {
				deliver_event(widget_under_mouse, &hev);
				widget_under_mouse->set_hover(true);
			}
			if (last_hover_widget) {
				hev.widget = NULL;
				hev.enter = false;
				hev.other = widget_under_mouse;
				deliver_event(last_hover_widget, &hev);
				last_hover_widget->set_hover(false);
			}
			last_hover_widget = widget_under_mouse;
		}

		if(mouse_button_state >= 0) {
			last_drag.x = mev->x;
			last_drag.y = mev->y;
		} else {

			if(!current_modal_window && focus_follows_mouse) {
				Container *root = (Container*)get_root_widget();
				Container::iterator iter = root->begin();
				while(iter != root->end()) {
					Widget *w = *iter++;
					if(w->hit_test(mev->x, mev->y)) {
						grab_focus(w);
						break;
					}
				}
			}

		}
		return;
	}

	MButtonEvent *bev;
	if((bev = dynamic_cast<MButtonEvent*>(e))) {
		receiver = mouse_grab_widget?mouse_grab_widget:root->get_child_at(bev->x, bev->y);
		
		if (current_modal_window && receiver->get_window() != current_modal_window)
			return;

		if(bev->pressed) {
			mouse_button_state = bev->button;
			last_drag.x = bev->x;
			last_drag.y = bev->y;
			last_press_x = bev->x;
			last_press_y = bev->y;
			mouse_press_widget = deliver_event(receiver, e);
			if (bev->button == MOUSE_LEFT)
				mouse_grab_widget = receiver;

			// show widget popup menu if the widget doesn't care about rightclicks
			if (!e->widget && bev->pressed && bev->button == MOUSE_RIGHT &&
				abs(bev->x - last_press_x) < 2 && abs(bev->y - last_press_y) < 2) {
				receiver->show_popup();
			}

			return;
		} else {
			mouse_button_state = -1;
			bev->press_x = last_press_x;
			bev->press_y = last_press_y;
			mouse_press_widget = 0;
			
			mouse_grab_widget = 0;	// note: grabbing is released on mouse release

			// houston, we have a click!
			if(bev->button >= MOUSE_LEFT && bev->button < MOUSE_RIGHT &&
					abs(bev->x - last_press_x) < 2 && abs(bev->y - last_press_y) < 2) {
				ClickEvent cev;
				cev.button = bev->button;
				cev.x = bev->x;
				cev.y = bev->y;
				cev.time = get_msec();
				deliver_event(receiver, &cev);
			}
		}

		deliver_event(receiver, e);
		return;
	}
}

void set_focus_mode(unsigned int fmode)
{
	focus_follows_mouse = (fmode == FOCUS_POINT);
}

unsigned int get_focus_mode()
{
	return focus_follows_mouse ? FOCUS_POINT : FOCUS_CLICK;
}

int get_button_state()
{
	return mouse_button_state;
}

Widget *get_button_press_widget()
{
	return mouse_press_widget;
}

IVec2 get_mouse_pos()
{
	return mouse_pos;
}

IVec2 get_last_drag_pos()
{
	return last_drag;
}

void grab_mouse(Widget *e)
{
	mouse_grab_widget = e;
}

void grab_focus(Widget *w)
{
	FocusEvent fev;
	
	if(focused_window == w)
		return;

	if(focused_window) {
		fev.focus = false;
		fev.widget = w;
		focused_window->handle_event(&fev);
	}

	fev.focus = true;
	fev.widget = focused_window;

	focused_window = w;
	focused_window->handle_event(&fev);
}

void grab_win_focus(Widget *w)
{
	if(dynamic_cast<Window*>(w) || dynamic_cast<WinFrame*>(w)) {
		return;
	}

	Widget *par = w;
	while((par = par->get_parent())) {
		Window *win = dynamic_cast<Window*>(par);
		if(win) {
			win->set_win_focus(w);
			return;
		}
	}
}

void invalidate_widget(Widget *w)
{
	if (focused_window == w) {
		focused_window = NULL;
	}
	if (mouse_press_widget == w) {
		mouse_press_widget = NULL;
	}
	if (focused_window == w) {
		focused_window = NULL;
	}
	if (last_hover_widget == w) {
		last_hover_widget = NULL;
	}
	if (!lock_destruct_queue)
		destruct_queue.remove(w);
}

void modalize_window(Window *w)
{
	w->set_modal(true);
	modal_windows.push(w);
	current_modal_window = w;
	grab_focus(w);
}

void close_last_modal_window()
{
	if (modal_windows.empty())	// sanity check
		return;
	modal_windows.pop();
	current_modal_window = modal_windows.empty()?NULL:modal_windows.top();
	if (current_modal_window)
		grab_focus(current_modal_window);
}

}	// namespace utk end
