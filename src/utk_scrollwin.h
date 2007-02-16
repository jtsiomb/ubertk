#ifndef UTK_SCROLLWIN_H_
#define UTK_SCROLLWIN_H_

#include <utk_widget.h>
#include <utk_drawable.h>

namespace utk {

class ScrollWin : public Drawable {
public:
	ScrollWin();
	virtual ~ScrollWin();

	virtual Widget *handle_event(Event *event);

	virtual void draw() const;
};

}

#endif	/* UTK_SCROLLWIN_H_ */
