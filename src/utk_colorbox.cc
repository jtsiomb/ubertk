#include "utk_colorbox.h"
#include "utk_hsv.h"

namespace utk {

void ColorBox::update()
{
	unsigned int *dst = pixels;
	float x, y;
	for (int j=0; j<img_h; j++)
	{
		for (int i=0; i<img_w; i++)
		{

			if (i == sel_s || j == sel_v)
			{
				*dst++ = 0xFFFFFFFF;
				continue;
			}
			x = (float) i / (float) (img_w - 1);
			y = (float) j / (float) (img_h - 1);
			*dst++ = pack_hsv(h, x, y);
		}
	}
}
void ColorBox::on_click(int x, int y)
{
	s = (float) x / (float) (img_w - 1);
	v = (float) y / (float) (img_h - 1);

	sel_s = x;
	sel_v = y;
}

void ColorBox::on_drag(int dx, int dy)
{
	sel_s += dx;
	sel_v += dy;
	if (sel_s < 0) sel_s = 0;
	if (sel_s >= img_w) sel_s = img_w - 1;
	if (sel_v < 0) sel_v = 0;
	if (sel_v >= img_h) sel_v = img_h - 1;

	s = (float) sel_s / (float) (img_w - 1);
	v = (float) sel_v / (float) (img_h - 1);
}

ColorBox::ColorBox(utk::Callback cb) : Image(150, 150, cb)
{
	sel_s = sel_v = h = s = v = 0;
}
ColorBox::~ColorBox()
{

}

float ColorBox::get_h() const
{
	return h;
}
float ColorBox::get_s() const
{
	return s;
}
float ColorBox::get_v() const
{
	return v;
}

void ColorBox::set_h(float h)
{
	this->h = h;
}

unsigned int ColorBox::get_selected_color() const
{
	return pack_hsv(h, s, v);
}

} // end namespace utk
