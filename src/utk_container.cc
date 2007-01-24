#include <algorithm>
#include "utk_container.h"
#include "utk_events.h"

namespace utk {

Container::Container()
{
	spacing = 4;
	border = 0;
	cache_idx = -1;
}

Container::~Container()
{
	if(herod_mode) {	// destroy children
		iterator iter = begin();
		while(iter != end()) {
			delete *iter++;
		}
		cont.clear();
	}
}

bool Container::handle_event(Event *event)
{
	const MouseEvent *mev;
	if((mev = dynamic_cast<const MouseEvent*>(event))) {
		iterator iter = begin();
		while(iter != end()) {
			Widget *w = *iter++;
			if(w->hit_test(mev->x, mev->y)) {
				return w->handle_event(event);
			}
		}
	}
	return false;
}

void Container::add_child(Widget *w)
{
	cont.push_back(w);
	w->set_parent(this);
	layout();
}

bool Container::remove_child(Widget *w)
{
	std::list<Widget*>::iterator iter = std::find(cont.begin(), cont.end(), w);
	if(iter == cont.end()) {
		return false;
	}
	if(herod_mode) {
		delete *iter;
	}
	cont.erase(iter);
	layout();
	return true;
}

void Container::raise_child(Widget *w)
{
	iterator iter = std::find(cont.begin(), cont.end(), w);
	if(iter != cont.end()) {
		cont.erase(iter);
		cont.push_front(w);
	}
}

void Container::sink_child(Widget *w)
{
	iterator iter = std::find(cont.begin(), cont.end(), w);
	if(iter != cont.end()) {
		cont.erase(iter);
		cont.push_back(w);
	}
}

void Container::set_spacing(int sp)
{
	spacing = sp;
}

int Container::get_spacing() const
{
	return spacing;
}

void Container::set_border(int b)
{
	border = b;
}

int Container::get_border() const
{
	return border;
}

Container::iterator Container::begin()
{
	return cont.begin();
}

Container::const_iterator Container::begin() const
{
	return cont.begin();
}

Container::iterator Container::end()
{
	return cont.end();
}

Container::const_iterator Container::end() const
{
	return cont.end();
}

std::list<Widget*>::iterator Container::get_iter_at(int idx) const
{
	Container *non_const_this = const_cast<Container*>(this);

	if(idx == cache_idx) return cache_iter;
	if(cache_idx > idx || cache_idx < 0) {
		cache_idx = 0;
		cache_iter = non_const_this->cont.begin();
	}

	while(cache_iter != cont.end() && cache_idx < idx) {
		cache_idx++;
		cache_iter++;
	}

	if(cache_iter == cont.end()) {
		cache_idx = -1;
		return non_const_this->cont.end();
	}
	return cache_iter;
}

Widget *Container::operator[](int idx)
{
	iterator iter = get_iter_at(idx);
	return iter == cont.end() ? 0 : *iter;
}

const Widget *Container::operator[](int idx) const
{
	iterator iter = get_iter_at(idx);
	return iter == cont.end() ? 0 : *iter;
}

size_t Container::size() const
{
	return cont.size();
}

void Container::draw() const
{
	const_iterator iter = begin();
	while(iter != end()) {
		(*iter++)->draw();
	}
}

int Container::get_width() const
{
	size_t w = 0;
	for (size_t i=0; i<cont.size(); i++)
	{
		w += (*this)[i]->get_width() + spacing;
	}
	w += 2 * border;
	return (int)w;
}

// TODO: also change the size of the container to fit the children

/* ---- layout function for the horizontal box container ---- */
void HBox::layout()
{
	int cur_x = border;
	for(size_t i=0; i<cont.size(); i++) {
		(*this)[i]->set_pos(cur_x, border);
		cur_x += (*this)[i]->get_width() + spacing;
	}
}

/* ---- layout function for the vertical box container ---- */
void VBox::layout()
{
	int cur_y = border;
	for(size_t i=0; i<cont.size(); i++) {
		(*this)[i]->set_pos(border, cur_y);
		cur_y += (*this)[i]->get_height() + spacing;
	}
}

void NullBox::layout()
{
}

}	// namespace utk end
