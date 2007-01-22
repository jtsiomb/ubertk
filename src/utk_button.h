#ifndef UBERTK_BUTTON_H_
#define UBERTK_BUTTON_H_

#include <utk_events.h>

namespace utk {

class Button : public Drawable {
public:
	Button(const char *txt = 0, utk::Callback cb = 0);
	virtual ~Button();
};

}

#endif	// UBERTK_BUTTON_H_
