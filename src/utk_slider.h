#ifndef UBERTK_SLIDER_H_
#define UBERTK_SLIDER_H_

#include <utk_scrollbar.h>
#include <utk_events.h>

namespace utk {

class Slider : public Scrollbar {
protected:
	float start, end;
	int vis_decimal;
	bool show_value;

public:
	Slider();
	virtual ~Slider();

	virtual Widget *handle_event(Event *event);

	virtual void set_min(float start);
	virtual void set_max(float end);
	virtual void set_range(float start, float end);
	virtual float get_min() const;
	virtual float get_max() const;

	virtual void set_vis_decimal(int count);
	virtual int get_vis_decimal() const;

	virtual void set_value(float val);
	virtual float get_value() const;
	virtual void operator=(float val);
	virtual operator float() const;

	virtual void draw() const;

	virtual void on_modify(Event *event);
};

Slider *create_slider(Widget *parent, float start, float end, Callback cb = 0, void *cdata = 0);
Slider *create_slider(Widget *parent, float start, float end, float *link);
Slider *create_slider(Widget *parent, float start, float end, int *link);
Slider *create_slider(Widget *parent, float start, float end, char *link, int bufsz);

}	// end namespace utk

#endif	// UBERTK_SLIDER_H_
