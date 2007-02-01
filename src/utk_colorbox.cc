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
}

ColorBox::ColorBox(utk::Callback cb) : Image(150, 150, cb)
{

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