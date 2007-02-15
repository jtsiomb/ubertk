#include "utk_huebox.h"
#include "utk_hsv.h"
#include <math.h>

namespace utk {

void HueBox::update()
{
	unsigned int *dst = pixels;
	for (int j=0; j<img_h; j++)
	{
		for (int i=0; i<img_w; i++)
		{
			float hue = (float) i / (float) (img_w - 1);
			if (i == sel_h)
			{
				hue += 0.5f;
			}
			hue = fmod(hue, 1.0f);
			*dst++ = pack_hsv(hue, 1.0f, 1.0f);
		}
	}
}
void HueBox::on_click(int x, int y)
{
	sel_h = x;
	h = (float) sel_h / (float) (img_w - 1);
	h = fmod(h, 1.0f);
}

#define CLAMP(x, a, b)	((x) < (a) ? (a) : ((x) > (b) ? (b) : (x)))

void HueBox::on_drag(int dx, int dy)
{
	int lmx = get_mouse_pos().x - get_global_pos().x;
	lmx = CLAMP(lmx, 0, img_w - 1);
	sel_h = lmx;
	h = (float) sel_h / (float) (img_w - 1);
	h = fmod(h, 1.0f);
}

HueBox::HueBox(utk::Callback cb) : Image(150, 20, cb)
{
	h = sel_h = 0;
	update();
}
HueBox::~HueBox()
{

}

float HueBox::get_h() const
{
	return h;
}
void HueBox::set_h(float h)
{
	this->h = h;
}

} // end namespace utk
