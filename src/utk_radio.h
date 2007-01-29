// utk_radio.h

#ifndef _UBERTK_RADIO_H_
#define _UBERTK_RADIO_H_

#include <utk_drawable.h>
#include <utk_events.h>

namespace utk {

class RadioBox : public Drawable {
protected:
	bool checked;
public:
	RadioBox(char *text = 0, utk::Callback cb = 0);
	virtual ~RadioBox();

	virtual Widget *handle_event(Event *event);

	virtual void draw() const;

	void check();
	bool is_checked() const;
};

}

#endif // ndef _UBERTK_RADIO_H_
