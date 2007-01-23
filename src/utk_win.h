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

	virtual bool handle_event(Event *event);

	virtual bool hit_test(int x, int y) const;

	virtual void draw() const;
};

}

#endif	// UBERTK_WIN_H_
