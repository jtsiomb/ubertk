// utk_chkbox.cc

#include "utk_chkbox.h"
#include "utk_gfx.h"

namespace utk {

CheckBox::CheckBox(const char *text)
{
	this->text = std::string(text);
	if(text) {
		this->size.x = gfx::text_width(text, 18) + gfx::text_spacing();
		this->size.y = gfx::text_spacing();
	} else {
		size.x = size.y = gfx::text_spacing();
	}
	checked = false;
}

CheckBox::~CheckBox() {}

Widget *CheckBox::handle_event(Event *event)
{
	ClickEvent *ce;
	if((ce = dynamic_cast<ClickEvent*>(event)) && hit_test(ce->x, ce->y))
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
	if (text.size())
	{
		gfx::color(0, 0, 0, 255);
		gfx::text(gpos.x + size.y, gpos.y + size.y, text.c_str(), 18);
	}

	Widget::draw();
}

void CheckBox::check()
{
	checked = true;
}

void CheckBox::uncheck()
{
	checked = false;
}

void CheckBox::set_checked(bool checked)
{
	this->checked = checked;
}

bool CheckBox::is_checked() const
{
	return checked;
}


CheckBox *create_checkbox(Widget *parent, const char *text, bool checked, Callback func, void *cdata)
{
	CheckBox *cbox = new CheckBox(text);
	cbox->set_callback(EVENT_MODIFY, func, cdata);
	parent->add_child(cbox);
	return cbox;
}

CheckBox *create_checkbox(Widget *parent, const char *text, bool checked, bool *link)
{
	// TODO
	return 0;
}

void destroy_checkbox(CheckBox *cbox)
{
	delete cbox;
}


} // end namespace utk
