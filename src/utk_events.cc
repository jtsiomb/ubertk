#include <list>
#include "ubertk.h"
#include "utk_events.h"

namespace utk {

static int focus_follows_mouse = 1;
static int mouse_button_state = -1;
static int last_press_x, last_press_y;
static Widget *mouse_press_widget;
static Widget *focused_window;
static IVec2 last_drag;
static IVec2 mouse_pos;

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

		destruct_queue.clear();
	}
}

static void handle_event(Event *e)
{
	Widget *root = get_root_widget();

	KeyboardEvent *kev;
	if((kev = dynamic_cast<KeyboardEvent*>(e))) {
		if(focused_window) {
			focused_window->handle_event(e);
		}
		return;
	}

	MMotionEvent *mev;
	if((mev = dynamic_cast<MMotionEvent*>(e))) {
		mouse_pos.x = mev->x;
		mouse_pos.y = mev->y;

		root->handle_event(e);

		if(mouse_button_state >= 0) {
			last_drag.x = mev->x;
			last_drag.y = mev->y;
		} else {

			if(focus_follows_mouse) {
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
		if(bev->pressed) {
			mouse_button_state = bev->button;
			last_drag.x = bev->x;
			last_drag.y = bev->y;
			last_press_x = bev->x;
			last_press_y = bev->y;
			mouse_press_widget = root->handle_event(e);
			return;
		} else {
			mouse_button_state = -1;
			bev->press_x = last_press_x;
			bev->press_y = last_press_y;

			// houston, we have a click!
			if(bev->button >= MOUSE_LEFT && bev->button < MOUSE_RIGHT &&
					abs(bev->x - last_press_x) < 2 && abs(bev->y - last_press_y) < 2) {
				ClickEvent cev;
				cev.button = bev->button;
				cev.x = bev->x;
				cev.y = bev->y;
				cev.time = get_msec();
				root->handle_event(&cev);
				return;	// don't generate both a click and a mouse-button event(?)
			}
		}

		root->handle_event(e);
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

void grab_focus(Widget *w)
{
	FocusEvent fev;

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

}	// namespace utk end
