#ifndef UTK_SCROLLWIN_H_
#define UTK_SCROLLWIN_H_

#include <utk_widget.h>
#include <utk_drawable.h>
#include <utk_scrollbar.h>

namespace utk {

enum {
	SCROLLBAR_HORIZONTAL	= 1,
	SCROLLBAR_VERTICAL		= 2,
	SCROLLBAR_BOTH			= 3
};
enum {
	SCROLLBAR_HIDE,
	SCROLLBAR_SHOW,
	SCROLLBAR_AUTO
};

class ScrollWin : public Drawable {
protected:
	Scrollbar *hbar, *vbar;
	int sbar_width;

	int vbar_state, hbar_state;

	bool check_vscroll_vis() const;
	bool check_hscroll_vis() const;

public:
	ScrollWin();
	virtual ~ScrollWin();

	virtual Widget *handle_event(Event *event);

	virtual void set_size(int w, int h);
	virtual void set_size(IVec2 sz);

	virtual void set_scrollbar_state(unsigned int which, int state);
	virtual int get_scrollbar_state(unsigned int which);

	virtual void set_hscroll(float val);
	virtual void set_vscroll(float val);
	virtual float get_hscroll() const;
	virtual float get_vscroll() const;

	virtual void draw() const;
};

}

#endif	/* UTK_SCROLLWIN_H_ */
