#include "utk_popup.h"
#include "utk_gfx.h"
#include "utk_common.h"

namespace utk {

Popup *create_popup(utk::Widget *parent)
{
	Popup	*popup = new Popup();
	parent->add_child(popup);
	return popup;
}

Popup::Popup()
{
	visible = false;
	focused = false;
	set_color(140, 140, 150);
	padding = 6;
}

void Popup::draw() const
{
	utk::IVec2	gpos = get_global_pos();
	
	utk::gfx::color_clamp(color.r, color.g, color.b, color.a);
	utk::gfx::bevel(gpos.x, gpos.y, gpos.x + size.x, gpos.y + size.y, utk::gfx::BEVEL_FILLBG, 2);

	Widget::draw();
}

}
