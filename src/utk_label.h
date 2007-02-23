#ifndef UBERTK_LABEL_H_
#define UBERTK_LABEL_H_

#include "utk_drawable.h"

namespace utk {


class Label : public Drawable {
protected:
	Color text_color;

public:
	Label(const char *txt = 0);
	virtual ~Label();

	virtual void set_text(const char *txt);

	virtual void set_text_color(int r, int g, int b, int a = 255);
	virtual void set_text_color(const Color &col);
	virtual const Color &get_text_color() const;

	virtual void draw() const;
};

Label *create_label(Widget *parent, const char *txt);

}

#endif	// UBERTK_LABEL_H_
