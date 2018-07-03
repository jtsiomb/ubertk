/*
ubertk is a flexible GUI toolkit targetted towards graphics applications.
Copyright (C) 2007 - 2013 John Tsiombikas <nuclear@member.fsf.org>,
                          Michael Georgoulopoulos <mgeorgoulopoulos@gmail.com>,
				          Kostas Michalopoulos <badsector@slashstone.com>

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
3. The name of the author may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

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
	delete [] pixels;
}

Widget *Image::handle_event(Event *event)
{
	ClickEvent *ce;
	IVec2 pos = get_global_pos();
	if((ce = dynamic_cast<ClickEvent*>(event)) && hit_test(ce->x, ce->y))
	{
		ce->x -= pos.x;
		ce->y -= pos.y;
		on_click(ce);
		invalidate();
		dragging = false;
		event->widget = this;
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
			invalidate();
			event->widget = this;
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
		invalidate();
		event->widget = this;
		return 0;
	}

	return 0;
}

void Image::invalidate()
{
	upd = true;
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

void Image::on_click(Event *ev)
{
}

void Image::on_motion(int x, int y)
{
}

void Image::on_drag(int dx, int dy)
{
}

} // end namespace utk
