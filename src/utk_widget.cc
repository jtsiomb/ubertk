#include "utk_widget.h"
#include "utk_container.h"

namespace utk {

Widget::Widget()
{
	visible = true;
	child = parent = 0;
	herod_mode = true;

	memset(callbacks, 0, EVENT_COUNT * sizeof *callbacks);
}

Widget::~Widget()
{
	if(herod_mode) {
		delete child;
	}
}

bool Widget::handle_event(Event *event)
{
	if(child) {
		return child->handle_event(event);
	}
	return false;
}

void Widget::destroy_children(bool enable)
{
	herod_mode = enable;
}

void Widget::set_pos(int x, int y)
{
	pos.x = x;
	pos.y = y;
}

IVec2 Widget::get_pos() const
{
	return pos;
}

IVec2 Widget::get_global_pos() const
{
	if(parent) {
		return pos + parent->get_global_pos();
	}
	return pos;
}

void Widget::set_size(int w, int h)
{
	size.x = w;
	size.y = h;
}

IVec2 Widget::get_size() const
{
	return size;
}

int Widget::get_width() const
{
	return size.x;
}

int Widget::get_height() const
{
	return size.y;
}

void Widget::set_visible(bool vis)
{
	visible = vis;
}

bool Widget::get_visible() const
{
	return visible;
}

void Widget::add_child(Widget *w)
{
	child = w;
	w->set_parent(this);
}

Widget *Widget::get_child()
{
	return child;
}

const Widget *Widget::get_child() const
{
	return child;
}


void Widget::set_parent(Widget *w)
{
	parent = w;
}

Widget *Widget::get_parent()
{
	return parent;
}

const Widget *Widget::get_parent() const
{
	return parent;
}

void Widget::rise()
{
	Container *cont;
	if(parent && (cont = dynamic_cast<Container*>(parent))) {
		cont->raise_child(this);
	}
}

void Widget::sink()
{
	Container *cont;
	if(parent && (cont = dynamic_cast<Container*>(parent))) {
		cont->sink_child(this);
	}
}

bool Widget::hit_test(int x, int y) const
{
	IVec2 gpos = get_global_pos();
	return visible && x >= gpos.x && x < gpos.x + size.x && y >= gpos.y && y < gpos.y + size.y;
}

void Widget::draw() const
{
	if(child) child->draw();
}

void Widget::set_callback(int event_type, Callback cbfunc)
{
	if(event_type >= 0 && event_type < EVENT_COUNT) {
		callbacks[event_type] = cbfunc;
	}
}

Callback Widget::get_callback(int event_type) const
{
	if(event_type < 0 || event_type > EVENT_COUNT) {
		return 0;
	}
	return callbacks[event_type];
}

void Widget::on_click(Event *event) {}
void Widget::on_focus(Event *event) {}


}	// end of namespace utk
