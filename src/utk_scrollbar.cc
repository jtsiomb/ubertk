/*
ubertk is a flexible GUI toolkit targetted towards graphics applications.
Copyright (C) 2007 - 2008 John Tsiombikas <nuclear@member.fsf.org>,
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

#include "utk_config.h"
#include "utk_scrollbar.h"
#include "utk_gfx.h"
#include "utk_common.h"

namespace utk {

IVec2 Scrollbar::get_cursor_tl() const
{
	IVec2 rel;
	int cwidth_half = cursor_width / 2;
	
	if(orient == HORIZONTAL) {
		rel.x = cursor_pos + track_start - cwidth_half;
		rel.y = border;
	} else {
		rel.x = border;
		rel.y = cursor_pos + track_start - cwidth_half;
	}
	
	return rel + get_global_pos();
}
IVec2 Scrollbar::get_cursor_br() const
{
	IVec2 rel;
	int cwidth_half = cursor_width / 2;

	if(orient == HORIZONTAL) {
		rel.x = cursor_pos + track_start + cwidth_half;
		rel.y = size.y - border;
	} else {
		rel.x = size.x - border;
		rel.y = cursor_pos + track_start + cwidth_half;
	}
	return rel + get_global_pos();
}

Scrollbar::Scrollbar()
{
	orient = HORIZONTAL;
	dragging = false;
	cursor_width = 20;
	cursor_pos = 0;
	set_border(2);
	set_size(100 + border * 2 + cursor_width, 20 + border * 2);
	set_color(128, 100, 80);
}

Scrollbar::~Scrollbar() {}

Widget *Scrollbar::handle_event(Event *event)
{
	// no child handled the event, either we do or return 0
	MButtonEvent *mb;
	if((mb = dynamic_cast<MButtonEvent*>(event)))
	{
		dragging = false;
		if (mb->pressed && hit_test(mb->x, mb->y))
		{
			if (mb->button == MOUSE_LEFT)
			{
				if (rect_test(get_cursor_tl(), get_cursor_br(), IVec2(mb->x, mb->y)))
					dragging = true;
			}
			mb->widget = this;
			return this;
		}
	}
	
	if (dynamic_cast<ClickEvent*>(event))
	{
		event->widget = this;
		dragging = false;
		return this;
	}

	MMotionEvent *mmev;
	if((mmev = dynamic_cast<MMotionEvent*>(event))) {
		if(dragging) {
			int dx;

			mmev->widget = this;
			
			if(orient == HORIZONTAL) {
				dx = mmev->x - get_last_drag_pos().x;
				if(dx < 0 && mmev->x > get_cursor_br().x) dx = 0;
				if(dx > 0 && mmev->x < get_cursor_tl().x) dx = 0;
			} else {
				dx = mmev->y - get_last_drag_pos().y;
				if(dx < 0 && mmev->y > get_cursor_br().y) dx = 0;
				if(dx > 0 && mmev->y < get_cursor_tl().y) dx = 0;
			}
			
			if(dx) {
				int	old_cursor_pos = cursor_pos;
				cursor_pos += dx;
				cursor_pos = cursor_pos < 0 ? 0 : (cursor_pos > track_len ? track_len : cursor_pos);

				if (old_cursor_pos != cursor_pos) {
					if(link_flt) {
						*link_flt = get_value();
					}

					on_modify(event);
					callback(event, EVENT_MODIFY);
				}
			}
		}
		event->widget = this;
		return this;
	}

	return this;
}

void Scrollbar::set_size(int w, int h)
{
	Widget::set_size(w, h);
	track_start = border + cursor_width / 2;
	track_len = (orient == HORIZONTAL ? w : h) - border * 2 - cursor_width;
}

void Scrollbar::set_border(int border)
{
	Drawable::set_border(border);
	track_start = border + cursor_width / 2;
	track_len = (orient == HORIZONTAL ? size.x : size.y) - border * 2 - cursor_width;
}

void Scrollbar::set_orientation(int orient)
{
	this->orient = orient;
}

int Scrollbar::get_orientation() const
{
	return orient;
}

void Scrollbar::set_cursor_width(int width)
{
	int max_width = (orient == HORIZONTAL ? size.x : size.y) - border * 2;
	cursor_width = MIN(width, max_width);
	set_size(size.x, size.y);	// force recalc of track_len
}

void Scrollbar::set_cursor_width(float percent)
{
	float width = (float)((orient == HORIZONTAL ? size.x : size.y) - border * 2);
	set_cursor_width((int)(width * percent));
}

int Scrollbar::get_cursor_width() const
{
	return cursor_width;
}

void Scrollbar::set_value(float val)
{
	int	old_cursor_pos = cursor_pos;
	val = val > 1.0 ? 1.0 : (val < 0.0 ? 0.0 : val);
	cursor_pos = (int)(val * track_len);

	if (old_cursor_pos == cursor_pos) return;

	// on set_value() the link should definitely be updated, but...
	if(link_flt) {
		*link_flt = val;
	}

	// ... should we also call the handlers?
	Event ev;
	ev.widget = this;

	on_modify(&ev);
	callback(&ev, EVENT_MODIFY);
}

float Scrollbar::get_value() const
{
	return (float)cursor_pos / (float)track_len;
}

void Scrollbar::operator=(float val)
{
	set_value(val);
}

Scrollbar::operator float() const
{
	return get_value();
}

void Scrollbar::draw() const
{
	IVec2 gpos = get_global_pos();

	gfx::color(color.r, color.g, color.b, color.a);
	gfx::rect(gpos.x, gpos.y, gpos.x + size.x, gpos.y + size.y);

	// draw cursor
	IVec2 tl = get_cursor_tl();
	IVec2 br = get_cursor_br();

	if(dragging) {
		gfx::color_clamp(light_color.r, light_color.g, light_color.b, light_color.a);
	} else {
		gfx::color_clamp(color.r, color.g, color.b, color.a);
	}
	gfx::bevel(tl.x, tl.y, br.x, br.y, gfx::BEVEL_FILLBG, 1);

	if(border) {
		gfx::color_clamp((int)(color.r * 1.25), (int)(color.g * 1.25), (int)(color.b * 1.25), color.a);

		gfx::line(gpos.x, gpos.y, gpos.x + size.x, gpos.y, border);
		gfx::line(gpos.x, gpos.y + size.y, gpos.x + size.x, gpos.y + size.y, border);
		gfx::line(gpos.x, gpos.y, gpos.x, gpos.y + size.y, border);
		gfx::line(gpos.x + size.x, gpos.y, gpos.x + size.x, gpos.y + size.y, border);
	}

	Widget::draw();
}


}
