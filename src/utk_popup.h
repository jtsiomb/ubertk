#ifndef UBERTK_POPUP_H_
#define UBERTK_POPUP_H_

#include <utk_widget.h>
#include <utk_drawable.h>

namespace utk {

class Popup : public Drawable
{
	public:
	Popup();
	
	virtual void draw() const;
};

Popup *create_popup(utk::Widget *parent);

}

#endif
