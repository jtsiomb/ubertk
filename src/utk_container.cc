#include <algorithm>
#include "utk_container.h"
#include "utk_events.h"
#include "utk_gfx.h"
#include <stack>

namespace utk {

Container::Container()
{
	spacing = 4;
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

Widget *Container::handle_event(Event *event)
{
	Widget *res = 0;

	const MouseEvent *mev;
	if((mev = dynamic_cast<const MouseEvent*>(event))) {
		iterator iter = begin();
		while(iter != end()) {
			Widget *w = *iter++;

			if(!res) {
				res = w->handle_event(event);
			} else {
				w->handle_event(event);
			}
			//if(w->hit_test(mev->x, mev->y)) {
			//	return w->handle_event(event);
			//}
		}
	}
	return res;
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

// EDO EINAI I DRAW FUNCTION !!!111oneoneeleven
void Container::draw() const
{
	std::list<Widget*>::const_reverse_iterator iter = cont.rbegin();
	while(iter != cont.rend()) {
		Widget *w = *iter++;
		if(w->is_visible()) {
			IVec2 cpos, csz;
			cpos = w->get_global_pos();
			csz = w->get_size();
			
			gfx::push_clip();
			gfx::mult_clip(cpos.x, cpos.y, cpos.x + csz.x, cpos.y + csz.y);
			w->draw();
			gfx::pop_clip();
		}
	}
}

int Container::get_width() const
{
	size_t w = 0;
	for (size_t i=0; i<cont.size(); i++)
	{
		w += (*this)[i]->get_width() + spacing;
	}
	w += 2 * padding;
	return (int)w;
}

/* ---- layout function for the horizontal box container ---- */
void HBox::layout()
{
	int max_y = get_size().y;
	int cur_x = padding;

	const_iterator iter = begin();
	while(iter != end()) {
		Widget *w = *iter++;

		w->set_pos(cur_x, padding);

		cur_x += w->get_width() + spacing;
		if(w->get_height() > max_y) {
			max_y = w->get_height();
		}
	}

	set_size(cur_x - spacing + padding, max_y);
}

/* ---- layout function for the vertical box container ---- */
void VBox::layout()
{
	int max_x = get_size().x;
	int cur_y = padding;

	const_iterator iter = begin();
	while(iter != end()) {
		Widget *w = *iter++;

		w->set_pos(padding, cur_y);
		
		cur_y += w->get_height() + spacing;
		if(w->get_width() > max_x) {
			max_x = w->get_width();
		}
	}

	set_size(max_x, cur_y - spacing + padding);
}

void NullBox::layout()
{
}

HBox *create_hbox(Widget *parent, int padding, int spacing)
{
	HBox *hbox = new HBox;
	hbox->set_padding(padding);
	hbox->set_spacing(spacing);
	parent->add_child(hbox);
	return hbox;
}

VBox *create_vbox(Widget *parent, int padding, int spacing)
{
	VBox *vbox = new VBox;
	vbox->set_padding(padding);
	vbox->set_spacing(spacing);
	parent->add_child(vbox);
	return vbox;
}

NullBox *create_nullbox(Widget *parent)
{
	NullBox *nbox = new NullBox;
	parent->add_child(nbox);
	return nbox;
}


}	// namespace utk end
