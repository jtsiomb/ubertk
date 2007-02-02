#include <math.h>
#include "utk_progress.h"
#include "utk_gfx.h"

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

#define MIN(a, b)	((a) < (b) ? (a) : (b))
#define MAX(a, b)	((a) > (b) ? (a) : (b))

void Progress::step()
{
	if(infinite) {
		set_value(fmod(get_value() + step_inc, 1.0));
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

	gfx::color(color.r, color.g, color.b, color.a);
	gfx::rect(gpos.x, gpos.y, gpos.x + size.x, gpos.y + size.y);

	float val = get_value();
	int bar_x = gpos.x + (int)(val * (size.x - border * 2)) + border;

	Color pbar_col(200, 100, 80, color.a);
	if(!infinite) {
		IVec2 pb_tl(gpos.x + border, gpos.y + border);
		IVec2 pb_br(bar_x, gpos.y + size.y - border);

		gfx::color(pbar_col.r, pbar_col.g, pbar_col.b, pbar_col.a);
		gfx::rect(pb_tl.x, pb_tl.y, pb_br.x, pb_br.y);

		Color ltcol = lighter_color(pbar_col);
		Color drcol = darker_color(pbar_col);

		gfx::color(ltcol.r, ltcol.g, ltcol.b, ltcol.a);
		gfx::rect(pb_tl.x, pb_tl.y, pb_br.x, pb_tl.y + 1);
		gfx::rect(pb_tl.x, pb_tl.y, pb_tl.x + 1, pb_br.y);

		gfx::color(drcol.r, drcol.g, drcol.b, drcol.a);
		gfx::rect(pb_tl.x, pb_br.y - 1, pb_br.x, pb_br.y);
		gfx::rect(pb_br.x - 1, pb_tl.y, pb_br.x, pb_br.y);
	} else {
		gfx::rect(MAX(gpos.x + border, bar_x - 30), gpos.y + border, bar_x, gpos.y + size.y - border);
	}

	if(border) {

		Color dark_color = darker_color(color);
		gfx::color(dark_color.r, dark_color.g, dark_color.b, dark_color.a);
		gfx::rect(gpos.x, gpos.y, gpos.x + size.x, gpos.y + 1);
		gfx::rect(gpos.x, gpos.y, gpos.x + 1, gpos.y + size.y);

		Color light_color = lighter_color(color);
		gfx::color(light_color.r, light_color.g, light_color.b, light_color.a);
		gfx::rect(gpos.x, gpos.y + size.y - 1, gpos.x + size.x, gpos.y + size.y);
		gfx::rect(gpos.x + size.x - 1, gpos.y, gpos.x + size.x, gpos.y + size.y);

		/*gfx::color_clamp((int)(color.r * 1.25), (int)(color.g * 1.25), (int)(color.b * 1.25), color.a);
		gfx::line(gpos.x, gpos.y, gpos.x + size.x, gpos.y, border);
		gfx::line(gpos.x, gpos.y + size.y, gpos.x + size.x, gpos.y + size.y, border);
		gfx::line(gpos.x, gpos.y, gpos.x, gpos.y + size.y, border);
		gfx::line(gpos.x + size.x, gpos.y, gpos.x + size.x, gpos.y + size.y, border);*/
	}

	Widget::draw();
}

}	// end namespace utk
