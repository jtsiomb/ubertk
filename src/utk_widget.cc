#include <typeinfo>
#include "utk_widget.h"
#include "utk_container.h"
#include "utk_gfx.h"

namespace utk {

Widget::Widget()
{
	padding = 0;
	visible = true;
	focused = false;
	child = parent = 0;
	herod_mode = true;
	name = 0;

	link_int = 0;
	link_flt = 0;
	link_str = 0;
	link_str_width = 0;

	memset(callbacks, 0, EVENT_COUNT * sizeof *callbacks);
}

Widget::~Widget()
{
	if (focused_window == this) {
		focused_window = NULL;
	}
	
	if(herod_mode) {
		delete child;
	}
}

void Widget::set_name(const char *name)
{
	char *tmp;
	if((tmp = (char*)malloc(strlen(name) + 1))) {
		strcpy(tmp, name);

		free(this->name);
		this->name = tmp;
	}
}

const char *Widget::get_name() const
{
	return name;
}

Widget *Widget::find_widget(const char *name)
{
	return (Widget*)((const Widget*)this)->find_widget(name);
}

const Widget *Widget::find_widget(const char *name) const
{
	if(strcmp(name, this->name) == 0) {
		return this;
	}
	return child ? child->find_widget(name) : 0;
}

static int digits(int x)
{
	x = abs(x);
	if(x < 10) return 1;
	return digits(x / 10) + 1;
}

const char *Widget::class_name() const
{
	const char *str = typeid(*this).name();

#ifdef __GNUC__
	static char buf[256];
	int ns = 0, num = 0;

	if(*str == 'N' && isdigit(str[1])) {
		ns = atoi(str + 1);
		str += digits(ns) + 1;

		memcpy(buf, str, ns);
		memcpy(buf + ns, "::", 3);
		str += ns;
	}

	if(isdigit(*str)) {
		num = atoi(str);
		str += digits(num);
	}

	char *ptr = buf + (ns ? ns + 2 : 0);
	memcpy(ptr, str, num);
	ptr[num] = 0;
	return buf;
#else
	return str;	// the name mangling is compiler-specific
#endif
}

Widget *Widget::handle_event(Event *event)
{
	if(child) {
		return child->handle_event(event);
	}
	return 0;
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

void Widget::set_pos(IVec2 pos)
{
	this->pos = pos;
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
	return get_size().x;
}

int Widget::get_height() const
{
	return get_size().y;
}

void Widget::set_padding(int pad)
{
	padding = pad;

	if(child) {
		IVec2 cpos = child->get_pos();
		child->set_pos(cpos.x + padding, cpos.y + padding);
	}
}

int Widget::get_padding() const
{
	return padding;
}

void Widget::show()
{
	visible = true;
}

void Widget::hide()
{
	visible = false;
}

bool Widget::is_visible() const
{
	return visible;
}

void Widget::set_focus(bool f)
{
	focused = f;
}

bool Widget::get_focus() const
{
	return focused;
}

void Widget::add_child(Widget *w)
{
	child = w;
	w->set_parent(this);

	IVec2 cpos = w->get_pos();
	w->set_pos(cpos.x + padding, cpos.y + padding);
}

Widget *Widget::get_child()
{
	return child;
}

const Widget *Widget::get_child() const
{
	return child;
}

bool Widget::remove_child(Widget *w)
{
	if(!child) return false;
	if(herod_mode) {
		delete child;
	}
	child = 0;
	return true;
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
	if(child && child->visible) {
		IVec2 cpos, csz;
		cpos = child->get_global_pos();
		csz = child->get_size();

		gfx::push_clip();
		gfx::mult_clip(cpos.x, cpos.y, cpos.x + csz.x, cpos.y + csz.y);
		child->draw();
		gfx::pop_clip();
	}
}

void Widget::set_link(int *ptr)
{
	link_int = ptr;
}

void Widget::set_link(float *ptr)
{
	link_flt = ptr;
}

void Widget::set_link(char *ptr, int width)
{
	link_str = ptr;
	link_str_width = width;
}

void Widget::set_callback(int event_type, Callback cbfunc, void *data)
{
	if(event_type >= 0 && event_type < EVENT_COUNT) {
		callbacks[event_type].func = cbfunc;
		callbacks[event_type].data = data;
	}
}

Callback Widget::get_callback(int event_type) const
{
	if(event_type < 0 || event_type > EVENT_COUNT) {
		return 0;
	}
	return callbacks[event_type].func;
}

void *Widget::get_callback_data(int event_type) const
{
	if(get_callback(event_type)) {
		return callbacks[event_type].data;
	}
	return 0;
}

void Widget::callback(Event *event, int event_type)
{
	CallbackClosure *cbc = &callbacks[event_type];
	if(cbc->func) {
		if(!event->widget) {
			event->widget = this;
		}
		cbc->func(event, cbc->data);
	}
}

void Widget::on_click(Event *event) {}
void Widget::on_focus(Event *event) {}
void Widget::on_modify(Event *event) {}


}	// end of namespace utk
