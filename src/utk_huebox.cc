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
void HueBox::on_click(Event *ev)
{
	int x = ((ClickEvent*)ev)->x;
	sel_h = x;
	h = (float) sel_h / (float) (img_w - 1);
	h = fmod(h, 1.0f);

	callback(ev, EVENT_MODIFY);
}

#define CLAMP(x, a, b)	((x) < (a) ? (a) : ((x) > (b) ? (b) : (x)))

void HueBox::on_drag(int dx, int dy)
{
	int lmx = get_mouse_pos().x - get_global_pos().x;
	lmx = CLAMP(lmx, 0, img_w - 1);
	sel_h = lmx;
	h = (float) sel_h / (float) (img_w - 1);
	h = fmod(h, 1.0f);

	utk::Event ev;
	callback(&ev, EVENT_MODIFY);
}

HueBox::HueBox(utk::Callback cb) : Image(150, 20, cb)
{
	h = 0.0f;
	sel_h = 0;
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

HueBox *create_huebox(Widget *parent, Callback func, void *cdata)
{
	HueBox *huebox = new HueBox;
	huebox->set_callback(EVENT_MODIFY, func, cdata);
	parent->add_child(huebox);
	return huebox;
}

} // end namespace utk
