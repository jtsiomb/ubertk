// utk_img.cc

#include "utk_img.h"
#include "utk_gfx.h"

namespace utk {

void Image::update()
{

}

Image::Image(int w, int h, utk::Callback cb)
{
	dragging = false;
	upd = false;
	if (w * h > 0)
	{
		pixels = new unsigned int[w * h];
		this->img_w = w;
		this->img_h = h;
	}
	else
	{
		this->img_w = this->img_h = 0;
		pixels = 0;
	}

	size.x = img_w;
	size.y = img_h;

	clear(0xFFFFFFFF);
}

Image::~Image()
{
	if (pixels)
		delete [] pixels;
}

Widget *Image::handle_event(Event *event)
{
	ClickEvent *ce;
	if((ce = dynamic_cast<ClickEvent*>(event)) && hit_test(ce->x, ce->y))
	{
		on_click(ce->x, ce->y);
		upd = true;
		return this;
	}

	MButtonEvent *mb;
	if((mb = dynamic_cast<MButtonEvent*>(event)))
	{
		dragging = false;
		if (mb->pressed && hit_test(mb->x, mb->y))
		{
			if (mb->button == MOUSE_LEFT)
			{
				dragging = true;
			}
			upd = true;
			return this;
		}
	}

	MMotionEvent *mmev;
	if((mmev = dynamic_cast<MMotionEvent*>(event))) 
	{
		if (dragging)
		{
			int dx = mmev->x - get_last_drag_pos().x;
			int dy = mmev->y - get_last_drag_pos().y;
			on_drag(dx, dy);
		}
		else
		{
			on_motion(mmev->x, mmev->y);
		}
		upd = true;
		return 0;
	}

	return 0;	
}

void Image::draw() const
{
	IVec2 gpos = get_global_pos();
	
	if (upd)
	{
		upd = false;
		((Image*)this)->update();
	}

	gfx::image(gpos.x, gpos.y, pixels, img_w, img_h);
}

void Image::clear(unsigned int c)
{
	for (int i=0; i<img_w * img_h; i++)
	{
		pixels[i] = c;
	}
}

void Image::on_click(int x, int y)
{

}

void Image::on_motion(int x, int y)
{

}

void Image::on_drag(int dx, int dy)
{
	
}

} // end namespace utk
