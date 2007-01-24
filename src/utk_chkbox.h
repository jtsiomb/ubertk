// utk_chkbox.h

#ifndef _UTK_CHKBOX_H_
#define _UTK_CHKBOX_H_

#include <utk_drawable.h>
#include <utk_events.h>

namespace utk {

class CheckBox : public Drawable {
protected:
	bool checked;
public:
	CheckBox(char *text = 0, utk::Callback cb = 0);
	virtual ~CheckBox();

	virtual Widget *handle_event(Event *event);

	virtual void draw() const;
};

}

#endif // ndef _UTK_CHKBOX_H_