#ifndef UBERTK_BUTTON_H_
#define UBERTK_BUTTON_H_

#include <utk_drawable.h>
#include <utk_events.h>

namespace utk {

class Button : public Drawable {
protected:
	bool pressed;

public:
	Button(const char *txt = 0, utk::Callback cb = 0);
	virtual ~Button();

	virtual bool handle_event(Event *event);

	virtual void draw() const;
};

}

#endif	// UBERTK_BUTTON_H_
