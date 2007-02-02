// utk_huebox.cc

#include "utk_huebox.h"

namespace utk {

void HueBox::update()
{
	unsigned int *dst = pixels;
	for (int j=0; j<img_h; j++)
	{
		for (int i=0; i<img_w; i++)
		{
			if (i == sel_h)
			{
				unsigned int line_color = 0xFF000000;
				if (j % 2)
				{
					line_color = 0xFFFFFFFF;
				}


			}
			float hue = (float) i / (float) (img_w - 1);

		}
	}
}
void HueBox::on_click(int x, int y)
{

}

HueBox::HueBox(utk::Callback cb = 0) : Image(128, 10, cb)
{
	h = sel_h = 0;
}
HueBox::~HueBox()
{

}

float HueBox::get_h() const
{

}
void HueBox::set_h(float h)
{

}

} // end namespace utk