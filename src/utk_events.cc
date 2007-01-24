#include "ubertk.h"
#include "utk_events.h"

namespace utk {

static int mouse_button_state = -1;
static int last_press_x, last_press_y;
static Widget *mouse_press_widget;
static IVec2 last_drag;

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
	Widget *root = get_root_widget();

	KeyboardEvent *kev;
	if((kev = dynamic_cast<KeyboardEvent*>(e))) {
		// send to focused window
		return;
	}

	MMotionEvent *mev;
	if((mev = dynamic_cast<MMotionEvent*>(e))) {
		root->handle_event(e);

		if(mouse_button_state >= 0) {
			last_drag.x = mev->x;
			last_drag.y = mev->y;
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
			if(bev->button >= MOUSE_LEFT && bev->button < MOUSE_RIGHT && bev->x == last_press_x && bev->y == last_press_y) {
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

int get_button_state()
{
	return mouse_button_state;
}

Widget *get_button_press_widget()
{
	return mouse_press_widget;
}

IVec2 get_last_drag_pos()
{
	return last_drag;
}


}	// namespace utk end
