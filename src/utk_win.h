#ifndef UBERTK_WIN_H_
#define UBERTK_WIN_H_

#include <utk_widget.h>
#include <utk_drawable.h>

namespace utk {

class Window : public Drawable {
protected:
	int tbar_height;

public:
	Window();
	virtual ~Window();

	virtual Widget *handle_event(Event *event);

	virtual void set_size(int w, int h);
	virtual void set_size(IVec2 sz);

	virtual void draw() const;

	friend class WinFrame;
};


class WinFrame : public Drawable {
protected:
	void update_geometry();

public:
	WinFrame(Widget *child);
	virtual ~WinFrame();

	virtual Widget *handle_event(Event *event);

	virtual void draw() const;

	friend class Window;
};


Window *create_window(Widget *parent, int x, int y, int w, int h, const char *title);


}

#endif	// UBERTK_WIN_H_
