#ifndef UBERTK_ENTRY_H_
#define UBERTK_ENTRY_H_

#include <utk_drawable.h>
#include <utk_events.h>

namespace utk {

class Entry : public Drawable {
protected:
	bool focus;
	int cursor;
	int view_first, view_last;

public:
	Entry(const char *txt = 0, utk::Callback cb = 0);
	virtual ~Entry();

	virtual Widget *handle_event(Event *event);

	virtual void draw() const;

	virtual void on_click(Event *event);
	virtual void on_modify(Event *event);
};

}

#endif	// UBERTK_ENTRY_H_
