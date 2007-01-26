// utk_chkbox.cc

#include "utk_chkbox.h"
#include "utk_gfx.h"

namespace utk {

CheckBox::CheckBox(char *text, utk::Callback cb)
{
	this->text = text;
	this->size.x = gfx::text_width(text, 18) + gfx::text_spacing();
	this->size.y = gfx::text_spacing();
	checked = false;
}

CheckBox::~CheckBox()
{

}

Widget *CheckBox::handle_event(Event *event)
{
	ClickEvent *ce;
	if((ce = dynamic_cast<ClickEvent*>(event)))
	{
		checked = !checked;
		return this;
	}

	return 0;
}

void CheckBox::draw() const
{
	IVec2 gpos = get_global_pos();

	// render box
	gfx::color(0, 0, 0, 255);
	gfx::rect(gpos.x, gpos.y, gpos.x + size.y, gpos.y + size.y);
	gfx::color(255, 255, 255, 255);
	gfx::rect(gpos.x + 2, gpos.y + 2, gpos.x + size.y - 2, gpos.y + size.y - 2);

	// render tick
	if (checked)
	{
		gfx::color(255, 100, 100, 255);
		gfx::line(gpos.x + 4, gpos.y + 4, gpos.x + size.y - 4, 
			gpos.y + size.y - 4, 5);
		gfx::line(gpos.x + size.y - 4, gpos.y + 4, gpos.x + 4, 
			gpos.y + size.y - 4, 5);
	}

	// render text
	if (text)
	{
		gfx::color(0, 0, 0, 255);
		gfx::text(gpos.x + size.y, gpos.y + size.y, text, 18);
	}

	Widget::draw();
}

} // end namespace utk
