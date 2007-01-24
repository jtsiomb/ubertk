// utk_scrollbar.h

#ifndef _UBERTK_SCROLLBAR_H_
#define _UBERTK_SCROLLBAR_H_

#include <utk_drawable.h>
#include <utk_events.h>

namespace utk {

class ScrollBar : public Drawable {
protected:
	int cursor_width;
	int cursor_height;
	int cursor_pos;

	bool dragging;

	IVec2 get_cursor_tl() const;
	IVec2 get_cursor_br() const;
public:
	ScrollBar(int cursor_width = 20, utk::Callback cb = 0);
	virtual ~ScrollBar();

	virtual bool handle_event(Event *event);

	virtual void draw() const;

	int get_percent() const;
};

}

#endif // ndef _UBERTK_SCROLLBAR_H_
