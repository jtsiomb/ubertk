#ifndef UBERTK_SLIDER_H_
#define UBERTK_SLIDER_H_

#include <utk_scrollbar.h>
#include <utk_events.h>

namespace utk {

class Slider : public ScrollBar {
protected:
	float start, end;
	int vis_decimal;
	bool show_value;
	float *link_float;
	int *link_int;

	void initialize();

public:
	Slider(float start = 0.0, float end = 1.0, utk::Callback cb = 0);
	Slider(float start, float end, float *link);
	Slider(float start, float end, int *link);
	virtual ~Slider();

	virtual void set_value(float val);
	virtual float get_value() const;
	virtual void operator=(float val);
	virtual operator float() const;

	virtual Widget *handle_event(Event *event);

	virtual void draw() const;
};

}	// end namespace utk

#endif	// UBERTK_SLIDER_H_
