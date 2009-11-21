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
#include <math.h>
#include "utk_progress.h"
#include "utk_gfx.h"
#include "utk_common.h"

namespace utk {

Progress::Progress(bool infinite)
{
	this->infinite = infinite;
	step_inc = 0.05;
}

Progress::~Progress() {}

Widget *Progress::handle_event(Event *event)
{
	return 0;
}

void Progress::set_progress_mode(int mode)
{
	infinite = mode == PROGRESS_INFINITE;
}

int Progress::get_progress_mode() const
{
	return infinite ? PROGRESS_INFINITE : PROGRESS_FINITE;
}

void Progress::step()
{
	if(infinite) {
		set_value(fmod(get_value() + step_inc, 1.0f));
	} else {
		float val = get_value() + step_inc;
		set_value(MIN(val, 1.0));
	}
}

void Progress::set_step(float sinc)
{
	step_inc = sinc;
}

float Progress::get_step() const
{
	return step_inc;
}

void Progress::draw() const
{
	IVec2 gpos = get_global_pos();

	gfx::color_clamp(color.r, color.g, color.b, color.a);
	if(border) {
		gfx::bevel(gpos.x, gpos.y, gpos.x + size.x, gpos.y + size.y, gfx::BEVEL_FILLBG | gfx::BEVEL_INSET, 1);
	} else {
		gfx::rect(gpos.x, gpos.y, gpos.x + size.x, gpos.y + size.y);
	}

	float val = get_value();
	int bar_x = gpos.x + (int)(val * (size.x - border * 2)) + border;

	Color pbar_col(74, 122, 171, color.a);
	if(!infinite) {
		IVec2 pb_tl(gpos.x + border, gpos.y + border);
		IVec2 pb_br(bar_x, gpos.y + size.y - border);

		gfx::color_clamp(pbar_col.r, pbar_col.g, pbar_col.b, pbar_col.a);
		
		gfx::rect(pb_tl.x, pb_tl.y, pb_br.x, pb_br.y);
		if(pb_tl.x < pb_br.x) {
			gfx::bevel(pb_tl.x, pb_tl.y, pb_br.x, pb_br.y, 0, 1);
		}
	} else {
		gfx::rect(MAX(gpos.x + border, bar_x - 30), gpos.y + border, bar_x, gpos.y + size.y - border);
	}

	Widget::draw();
}

}	// end namespace utk
