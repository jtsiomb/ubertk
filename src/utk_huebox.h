#ifndef UBERTK_HUEBOX_H_
#define UBERTK_HUEBOX_H_

#include <utk_img.h>

namespace utk {

class HueBox : public Image {
protected:
	float h;
	int sel_h;

	virtual void update();

	virtual void on_click(int x, int y);
	//virtual void on_motion(int x, int y);
	virtual void on_drag(int dx, int dy);
public:
	HueBox(utk::Callback cb = 0);
	virtual ~HueBox();

	float get_h() const;
	void set_h(float h);
};

}

#endif // ndef _UBERTK_HUEBOX_H_
