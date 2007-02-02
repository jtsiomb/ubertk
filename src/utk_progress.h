#ifndef UBERTK_PROGRESS_H_
#define UBERTK_PROGRESS_H_

#include <utk_scrollbar.h>
#include <utk_events.h>

namespace utk {

enum {PROGRESS_FINITE, PROGRESS_INFINITE};

class Progress : public Scrollbar {
protected:
	bool infinite;
	float step_inc;

public:
	Progress(bool infinite = false);
	virtual ~Progress();

	virtual Widget *handle_event(Event *event);

	virtual void set_progress_mode(int mode);
	virtual int get_progress_mode() const;

	virtual void step();
	virtual void set_step(float sinc);
	virtual float get_step() const;

	virtual void draw() const;
};

}	// end namespace utk

#endif	// UBERTK_PROGRESS_H_
