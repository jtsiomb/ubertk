// utk_img.h

#ifndef _UBERTK_IMAGE_H_
#define _UBERTK_IMAGE_H_

#include <utk_drawable.h>
#include <utk_events.h>

namespace utk {

class Image : public Drawable {
protected:
	int img_w, img_h;
	unsigned int *pixels;
	bool dragging;
	mutable bool upd;

	virtual void update();

	virtual void on_click(int x, int y);
	virtual void on_motion(int x, int y);
	virtual void on_drag(int dx, int dy);
public:
	Image(int w = 100, int h = 100, utk::Callback cb = 0);
	virtual ~Image();

	virtual Widget *handle_event(Event *event);

	virtual void draw() const;
	void clear(unsigned int c);
};

}

#endif // ndef _UBERTK_IMAGE_H_
