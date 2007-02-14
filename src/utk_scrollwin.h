#ifndef UTK_SCROLLWIN_H_
#define UTK_SCROLLWIN_H_

#include <utk_widget.h>
#include <utk_drawable.h>

namespace utk {

class ScrollWin : public Drawable {
protected:
	IVec2 view_pos, view_size;

public:
	ScrollWin();
	virtual ~ScrollWin();

	virtual Widget *handle_event(Event *event);

	virtual IVec2 get_global_pos() const;

	virtual void set_view_pos(int x, int y);
	virtual void set_view_pos(IVec2 pos);
	virtual IVec2 get_view_pos() const;

	virtual void set_view_size(int w, int h);
	virtual void set_view_size(IVec2 sz);
	virtual IVec2 get_view_size() const;
	virtual int get_view_width() const;
	virtual int get_view_height() const;

	virtual void draw() const;
};

}

#endif	/* UTK_SCROLLWIN_H_ */
