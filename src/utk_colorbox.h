// utk_tball.h

#ifndef _UBERTK_COLORBOX_H_
#define _UBERTK_COLORBOX_H_

#include <utk_img.h>

namespace utk {

class ColorBox : public Image {
protected:
	float h, s, v;
	int sel_s, sel_v;

	virtual void update();

	virtual void on_click(int x, int y);
	//virtual void on_motion(int x, int y);
	virtual void on_drag(int dx, int dy);
public:
	ColorBox(utk::Callback cb = 0);
	virtual ~ColorBox();

	float get_h() const;
	float get_s() const;
	float get_v() const;

	void set_h(float h);

	unsigned int get_selected_color() const;
};

}

#endif // ndef _UBERTK_COLORBOX_H_
