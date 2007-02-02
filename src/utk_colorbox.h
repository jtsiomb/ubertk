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

	virtual void set_color(int r, int g, int b, int a = 255);
	virtual void set_color(const Color &col);
	virtual unsigned int get_packed_color() const;
};

}

#endif // ndef _UBERTK_COLORBOX_H_
