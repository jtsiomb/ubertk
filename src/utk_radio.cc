// utk_radio.cc

#include "utk_radio.h"
#include "utk_container.h"
#include "utk_gfx.h"

namespace utk {

RadioBox::RadioBox(char *text, utk::Callback cb)
{
	this->text = text;
	if (this->text.size())
	{
		this->size.x = gfx::text_width(text, 18) + gfx::text_spacing();
		this->size.y = gfx::text_spacing();
	}
	else
	{
		size.x = size.y = gfx::text_spacing();
	}
	checked = false;
}
RadioBox::~RadioBox()
{

}

Widget *RadioBox::handle_event(Event *event)
{
	ClickEvent *ce;
	if((ce = dynamic_cast<ClickEvent*>(event)) && hit_test(ce->x, ce->y))
	{
		check();
		return this;
	}

	return 0;
}

void RadioBox::draw() const
{
	IVec2 gpos = get_global_pos();

	// render box
	gfx::color(0, 0, 0, 255);
	gfx::circle(gpos.x, gpos.y, gpos.x + size.y, gpos.y + size.y, false);
	gfx::color(255, 255, 255, 255);
	gfx::circle(gpos.x + 2, gpos.y + 2, gpos.x + size.y - 2, gpos.y + size.y - 2, false);

	// render tick
	if (checked)
	{
		gfx::color(0, 0, 0, 255);
		gfx::circle(gpos.x + 4, gpos.y + 4, gpos.x + size.y - 4, 
			gpos.y + size.y - 4, false);
	}

	// render text
	if (text.size())
	{
		gfx::color(0, 0, 0, 255);
		gfx::text(gpos.x + size.y, gpos.y + size.y, text.c_str(), 18);
	}

	Widget::draw();
}

void RadioBox::check()
{
	Container *cont_par = dynamic_cast<Container*> (parent);
	if (cont_par)
	{
		for (unsigned int i=0; i<cont_par->size(); i++)
		{
			RadioBox *rb = dynamic_cast<RadioBox*> ((*cont_par)[i]);
			if (rb)
			{
				rb->checked = false;
			}
		}
	}
	checked = true;
}

bool RadioBox::is_checked() const
{
	return checked;
}

}
