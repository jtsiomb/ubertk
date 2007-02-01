// utk_tball.h

#ifndef _UBERTK_TBALL_H_
#define _UBERTK_TBALL_H_

#include <utk_img.h>

namespace utk {

class TrackBall : public Image {
protected:
	float phi, theta;

	virtual void update();

	virtual void on_click(int x, int y);
	virtual void on_motion(int x, int y);
	virtual void on_drag(int dx, int dy);
public:
	TrackBall(int w = 100, int h = 100, utk::Callback cb = 0);
	virtual ~TrackBall();

	float get_phi() const;
	float get_theta() const;
};

}

#endif // ndef _UBERTK_TBALL_H_
