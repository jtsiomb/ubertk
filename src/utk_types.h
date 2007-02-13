#ifndef UBERTK_TYPES_H_
#define UBERTK_TYPES_H_

namespace utk {

enum {HORIZONTAL, VERTICAL};

class IVec2 {
public:
	int x, y;

	IVec2(int x = 0, int y = 0);
	IVec2 operator+(const IVec2 &v) const;
	IVec2 operator-(const IVec2 &v) const;
};

bool rect_test(const IVec2 &tl, const IVec2 &br, const IVec2 &pos);

class Color {
public:
	int r, g, b, a;

	Color(int r = 0, int g = 0, int b = 0, int a = 255);
};

Color lighter_color(const Color &col);
Color darker_color(const Color &col);

class Rect {
public:
	int x1, y1, x2, y2;

	Rect();
	Rect(int x1, int y1, int x2, int y2);
};

}

#endif	// UBERTK_TYPES_H_
