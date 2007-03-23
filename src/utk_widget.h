#ifndef UBERTK_WIDGET_H_
#define UBERTK_WIDGET_H_

#include <utk_types.h>
#include <utk_events.h>

namespace utk {

class Event;

class Widget {
protected:
	char *name;
	IVec2 pos, size;
	int padding;

	Widget *child, *parent;
	bool visible;
	bool focused;

	bool herod_mode;

	CallbackClosure callbacks[EVENT_COUNT];

public:
	Widget();
	virtual ~Widget();
	
	virtual void set_name(const char *name);
	virtual const char *get_name() const;

	virtual Widget *find_widget(const char *name);
	virtual const Widget *find_widget(const char *name) const;

	virtual const char *class_name() const;

	virtual Widget *handle_event(Event *event);

	virtual void destroy_children(bool enable);

	virtual void set_pos(int x, int y);
	virtual void set_pos(IVec2 pos);
	virtual IVec2 get_pos() const;
	virtual IVec2 get_global_pos() const;

	virtual void set_size(int w, int h);
	virtual IVec2 get_size() const;
	virtual int get_width() const;
	virtual int get_height() const;

	virtual void set_padding(int pad);
	virtual int get_padding() const;

	virtual void show();
	virtual void hide();
	virtual bool is_visible() const;

	virtual void set_focus(bool f);
	virtual bool get_focus() const;

	virtual void add_child(Widget *w);
	virtual Widget *get_child();
	virtual const Widget *get_child() const;
	virtual bool remove_child(Widget *w = 0);

	virtual void set_parent(Widget *w);
	virtual Widget *get_parent();
	virtual const Widget *get_parent() const;

	virtual void rise();
	virtual void sink();

	virtual bool hit_test(int x, int y) const;

	virtual void draw() const;

	virtual void set_callback(int event_type, Callback cbfunc, void *data = 0);
	virtual Callback get_callback(int event_type) const;
	virtual void *get_callback_data(int event_type) const;

	virtual void callback(Event *event, int event_type);

	virtual void on_click(Event *event);
	virtual void on_focus(Event *event);
	virtual void on_modify(Event *event);
};

}

#endif	// UBERTK_WIDGET_H_
