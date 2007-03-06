#ifndef UBERTK_ENTRY_H_
#define UBERTK_ENTRY_H_

#include <utk_drawable.h>
#include <utk_events.h>

namespace utk {

class Entry : public Drawable {
protected:
	bool focus;
	int cursor;
	int vfirst;

public:
	Entry(const char *txt = 0, utk::Callback cb = 0);
	virtual ~Entry();

	virtual Widget *handle_event(Event *event);

	virtual void draw() const;
};

Entry *create_entry(Widget *parent, const char *text, int width = 100, Callback func = 0, void *cdata = 0);
void destroy_entry(Entry *en);

}

#endif	// UBERTK_ENTRY_H_
