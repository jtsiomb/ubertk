#ifndef UBERTK_BUTTON_H_
#define UBERTK_BUTTON_H_

#include <utk_drawable.h>
#include <utk_events.h>

namespace utk {

class Button : public Drawable {
protected:
	bool pressed;
	bool flat;

public:
	Button(const char *txt = 0, Callback cb = 0);
	virtual ~Button();

	virtual Widget *handle_event(Event *event);

	virtual void draw() const;

	virtual void set_flat(bool flat);
	virtual bool is_flat() const;
};

Button *create_button(Widget *parent, const char *text, Callback func = 0, void *cdata = 0);
Button *create_button(Widget *parent, const char *text, int xsz, int ysz = 0, Callback func = 0, void *cdata = 0);

}	// end of namespace utk

#endif	// UBERTK_BUTTON_H_
