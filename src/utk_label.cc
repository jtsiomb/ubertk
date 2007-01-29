#include "utk_label.h"
#include "utk_gfx.h"

namespace utk {

Label::Label(const char *txt)
{
	if(txt) set_text(txt);
	text_color = Color(0, 0, 0, 255);
	set_size(0, gfx::text_spacing());
}

Label::~Label() {}

void Label::set_text_color(int r, int g, int b, int a)
{
	text_color = Color(r, g, b, a);
}

void Label::set_text_color(const Color &col)
{
	text_color = col;
}

const Color &Label::get_text_color() const
{
	return text_color;
}

void Label::draw() const
{
	if(text.size()) {
		IVec2 gpos = get_global_pos();
		gfx::color(text_color.r, text_color.g, text_color.b, text_color.a);
		gfx::text(gpos.x, gpos.y + gfx::text_spacing() + 1, text.c_str(), 18);
	}
}

}
