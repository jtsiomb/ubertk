#ifndef UBERTK_WIN_H_
#define UBERTK_WIN_H_

#include <utk_widget.h>
#include <utk_drawable.h>

namespace utk {

class Window : public Drawable {
protected:
	int tbar_height;
	bool shaded;

public:
	Window();
	virtual ~Window();

	virtual Widget *handle_event(Event *event);

	virtual void set_size(int w, int h);
	virtual void set_size(IVec2 sz);

	virtual void rise();
	virtual void sink();

	virtual void draw() const;

	friend class WinFrame;
};


class WinFrame : public Drawable {
protected:
	unsigned int last_click; // to detect double-clicks
	bool shaded;
	IVec2 orig_size;	// used when shaded

	void update_geometry();
	void set_shade(bool shade);

public:
	WinFrame(Widget *child);
	virtual ~WinFrame();

	virtual Widget *handle_event(Event *event);

	virtual bool is_visible() const;

	virtual void draw() const;

	friend class Window;
};


Window *create_window(Widget *parent, int x, int y, int w, int h, const char *title);
void destroy_window(Widget *w);

}	// end of namespace utk

#endif	// UBERTK_WIN_H_
