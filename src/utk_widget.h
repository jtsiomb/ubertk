#ifndef UBERTK_WIDGET_H_
#define UBERTK_WIDGET_H_

#include <utk_types.h>
#include <utk_events.h>

namespace utk {

class Event;

class Widget {
protected:
	IVec2 pos, size;

	Widget *child, *parent;
	bool visible;

	bool herod_mode;

	Callback callbacks[EVENT_COUNT];

public:
	Widget();
	virtual ~Widget();

	virtual bool handle_event(Event *event);

	virtual void destroy_children(bool enable);

	virtual void set_pos(int x, int y);
	virtual IVec2 get_pos() const;
	virtual IVec2 get_global_pos() const;

	virtual void set_size(int w, int h);
	virtual IVec2 get_size() const;
	virtual int get_width() const;
	virtual int get_height() const;

	virtual void set_visible(bool vis);
	virtual bool get_visible() const;

	virtual void add_child(Widget *w);
	virtual Widget *get_child();
	virtual const Widget *get_child() const;

	virtual void set_parent(Widget *w);
	virtual Widget *get_parent();
	virtual const Widget *get_parent() const;

	virtual void rise();
	virtual void sink();

	virtual bool hit_test(int x, int y) const;

	virtual void draw() const;

	virtual void set_callback(int event_type, Callback cbfunc);
	virtual Callback get_callback(int event_type) const;

	virtual void on_click(Event *event);
	virtual void on_focus(Event *event);
};

}

#endif	// UBERTK_WIDGET_H_
