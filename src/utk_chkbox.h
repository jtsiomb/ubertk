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
	CheckBox(const char *text = 0);
	virtual ~CheckBox();

	virtual Widget *handle_event(Event *event);

	virtual void draw() const;

	virtual void check();
	virtual void uncheck();
	virtual void set_checked(bool checked);
	virtual bool is_checked() const;
};

CheckBox *create_checkbox(Widget *parent, const char *text, bool checked, Callback func = 0, void *cdata = 0);
CheckBox *create_checkbox(Widget *parent, const char *text, bool checked, bool *link);
void destroy_checkbox(CheckBox *cbox);

}

#endif // ndef _UTK_CHKBOX_H_
