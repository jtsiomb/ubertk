#include "ubertk.h"
#include "utk_events.h"

namespace utk {

static int mouse_button_state = -1;
static IVec2 last_drag;

Event::Event() {}
Event::~Event() {}

MouseEvent::MouseEvent()
{
	x = y = 0;
}

MouseEvent::MouseEvent(int x, int y)
{
	this->x = x;
	this->y = y;
}

MMotionEvent::MMotionEvent() {}
MMotionEvent::MMotionEvent(int x, int y) : MouseEvent(x, y) {}

MButtonEvent::MButtonEvent()
{
	x = y = button = 0;
}

MButtonEvent::MButtonEvent(int bn, int x, int y)
	: MouseEvent(x, y)
{
	button = bn;
}

KeyboardEvent::KeyboardEvent(int key)
{
	this->key = key;
}


void event(const Event *e)
{
	Widget *root = get_root_widget();

	const KeyboardEvent *kev;
	if((kev = dynamic_cast<const KeyboardEvent*>(e))) {
		// send to focused window
		return;
	}

	const MMotionEvent *mev;
	if((mev = dynamic_cast<const MMotionEvent*>(e))) {
		root->handle_event(e);

		if(mouse_button_state >= 0) {
			last_drag.x = mev->x;
			last_drag.y = mev->y;
		}
		return;
	}

	const MButtonEvent *bev;
	if((bev = dynamic_cast<const MButtonEvent*>(e))) {
		if(bev->pressed) {
			mouse_button_state = bev->button;
			last_drag.x = bev->x;
			last_drag.y = bev->y;
		} else {
			mouse_button_state = -1;
		}

		root->handle_event(e);
		return;
	}
}

int get_button_state()
{
	return mouse_button_state;
}

IVec2 get_last_drag_pos()
{
	return last_drag;
}


}	// namespace utk end
