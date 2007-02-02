#ifndef UBERTK_SCROLLBAR_H_
#define UBERTK_SCROLLBAR_H_

#include <utk_drawable.h>
#include <utk_events.h>

namespace utk {

class Scrollbar : public Drawable {
protected:
	int cursor_width;
	int track_len, track_start;
	int cursor_pos;

	bool dragging;
	int orient;

	IVec2 get_cursor_tl() const;
	IVec2 get_cursor_br() const;

public:
	explicit Scrollbar(utk::Callback cb = 0);
	virtual ~Scrollbar();

	virtual Widget *handle_event(Event *event);

	virtual void set_size(int w, int h);
	virtual void set_border(int border);
	
	virtual void set_value(float val);
	virtual float get_value() const;
	virtual void operator=(float val);
	virtual operator float() const;

	virtual void draw() const;
};

}

#endif // ndef UBERTK_SCROLLBAR_H_
