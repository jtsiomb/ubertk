#ifndef UBERTK_DRAWABLE_H_
#define UBERTK_DRAWABLE_H_

#include <utk_widget.h>
#include <utk_types.h>

namespace utk {

class Drawable : public Widget {
protected:
	Color color;
	int border;
	char *text;

public:
	Drawable();
	virtual ~Drawable();

	virtual void set_text(const char *text);
	virtual const char *get_text() const;

	virtual void set_color(int r, int g, int b, int a = 255);
	virtual void set_color(const Color &col);
	virtual const Color &get_color() const;

	virtual void set_alpha(int a);
	virtual int get_alpha() const;

	virtual void set_border(int border);
	virtual int get_border() const;
};

}

#endif	// UBERTK_DRAWABLE_H_
