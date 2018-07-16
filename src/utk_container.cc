/*
ubertk is a flexible GUI toolkit targetted towards graphics applications.
Copyright (C) 2007 - 2013 John Tsiombikas <nuclear@member.fsf.org>,
                          Michael Georgoulopoulos <mgeorgoulopoulos@gmail.com>,
				          Kostas Michalopoulos <badsector@slashstone.com>

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
3. The name of the author may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include <string.h>
#include <stack>
#include <algorithm>
#include "utk_container.h"
#include "utk_events.h"
#include "utk_gfx.h"

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

const Widget *Container::find_widget(const char *name) const
{
	if (get_name() && !strcmp(get_name(), name))
		return this;

	const_iterator iter = begin();
	while(iter != end()) {
		if((*iter)->get_name() && strcmp((*iter)->get_name(), name) == 0) {
			return *iter;
		}
		iter++;
	}
	return 0;
}

void Container::layout()
{
	Widget *ptr = parent;

	while(ptr && !dynamic_cast<Container*>(ptr)) {
		ptr = ptr->get_parent();
	}

	if(ptr) {
		((Container*)ptr)->layout();
	}
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
	cache_idx = -1;
	layout();
	return true;
}

Widget *Container::get_child_at(int x, int y)
{
	if(!visible) return 0;

	iterator iter = cont.begin();
	while (iter != cont.end()) {
		Widget	*w = *iter++;
		if (w->hit_test(x, y)) {
			return w->get_child_at(x, y);
		}
	}
	return this;
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

void Container::clear()
{
	while(!cont.empty()) {
		remove_child(*cont.begin());
	}
}

size_t Container::size() const
{
	return cont.size();
}

bool Container::empty() const
{
	return cont.empty();
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


/* ---- layout function for the horizontal box container ---- */
void HBox::layout()
{
	int max_y = 0;
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

	if(cont.size() > 0) {
		cur_x -= spacing;
	}

	set_size(cur_x + padding, max_y);
	Container::layout();
}

/* ---- layout function for the vertical box container ---- */
void VBox::layout()
{
	int max_x = 0;
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

	if(cont.size() > 0) {
		cur_y -= spacing;
	}

	set_size(max_x, cur_y + padding);
	Container::layout();
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
