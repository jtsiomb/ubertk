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

	float r, g, b;
	hsv_to_rgb(&r, &g, &b, h, s, v);
	Drawable::set_color((int)(r * 255.0), (int)(g * 255.0), (int)(b * 255.0), color.a);
}

#define CLAMP(x, a, b)	((x) < (a) ? (a) : ((x) > (b) ? (b) : (x)))

void ColorBox::on_drag(int dx, int dy)
{
	IVec2 gpos = get_global_pos();
	IVec2 mpos = get_mouse_pos();

	sel_s = CLAMP(mpos.x - gpos.x, 0, img_w - 1);
	sel_v = CLAMP(mpos.y - gpos.y, 0, img_w - 1);

	s = (float)sel_s / (float)(img_w - 1);
	v = (float)sel_v / (float)(img_h - 1);

	float r, g, b;
	hsv_to_rgb(&r, &g, &b, h, s, v);
	Drawable::set_color((int)(r * 255.0), (int)(g * 255.0), (int)(b * 255.0), color.a);
}

ColorBox::ColorBox(utk::Callback cb) : Image(150, 150, cb)
{
	sel_s = sel_v = 0;
	set_color_hsv(0, 0, 0);
	update();
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

	float r, g, b;
	hsv_to_rgb(&r, &g, &b, h, s, v);
	Drawable::set_color((int)(r * 255.0), (int)(g * 255.0), (int)(b * 255.0), color.a);
}

void ColorBox::set_color(int r, int g, int b, int a)
{
	Drawable::set_color(r, g, b, a);
	rgb_to_hsv((float)r / 255.0, (float)g / 255.0, (float)b / 255.0, &h, &s, &v);
}

void ColorBox::set_color_hsv(int h, int s, int v, int a)
{
	this->s = s;
	this->v = v;
	color.a = a;
	set_h(h);
}

void ColorBox::set_color(const Color &col)
{
	Drawable::set_color(col);
	rgb_to_hsv((float)col.r / 255.0, (float)col.g / 255.0, (float)col.b / 255.0, &h, &s, &v);
}

unsigned int ColorBox::get_packed_color() const
{
	return pack_hsv(h, s, v);
}

} // end namespace utk
