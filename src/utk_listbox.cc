#include "utk_listbox.h"
#include "utk_label.h"
#include "utk_gfx.h"
#include "utk_common.h"

namespace utk {

ListBox::ListBox()
{
	vbox = new VBox;
	add_child(vbox);
	sel = -1;
}

ListBox::~ListBox()
{
	delete vbox;	// vbox will delete its children in herod mode (default)
}

Widget *ListBox::handle_event(Event *event)
{
	ClickEvent *cev;
	if((cev = dynamic_cast<ClickEvent*>(event)) && hit_test(cev->x, cev->y) && !cev->widget) {
		int i = 0;
		VBox::iterator iter = vbox->begin();
		while(iter != vbox->end()) {
			if((*iter)->hit_test((*iter)->get_global_pos().x, cev->y)) {
				select(i);

				cev->widget = this;
				callback(cev, EVENT_MODIFY);
				on_modify(cev);

				return this;
			}
			iter++;
			i++;
		}
	}
	return ScrollWin::handle_event(event);
}

void ListBox::add_item(Widget *item, int pos)
{
	if(pos < 0 || pos > (int)vbox->size()) {
		vbox->add_child(item);
	} else {
		// TODO: insert in specific position
	}
}

void ListBox::add_item(const char *str, int pos)
{
	add_item(new Label(str), pos);
}

void ListBox::remove_item(int pos)
{
	// TODO
}

void ListBox::select(int pos)
{
	if(pos < 0) {
		sel = -1;
	} else if(pos >= (int)vbox->size()) {
		sel = vbox->size() - 1;
	} else {
		sel = pos;
	}
}

void ListBox::select(const char *str)
{
	for(size_t i=0; i<vbox->size(); i++) {
		Drawable *w = dynamic_cast<Drawable*>((*vbox)[i]);
		if(w && strcmp(w->get_text(), str) == 0) {
			sel = i;
			break;
		}
	}
	sel = -1;
}

int ListBox::get_selected() const
{
	return sel;
}

const char *ListBox::get_selected_text() const
{
	Drawable *w;
	if((w = dynamic_cast<Drawable*>((*vbox)[sel]))) {
		return w->get_text();
	}
	return 0;
}

void ListBox::set_spacing(int sp)
{
	vbox->set_spacing(sp);
}

int ListBox::get_spacing() const
{
	return vbox->get_spacing();
}

void ListBox::clear()
{
	vbox->clear();
}

void ListBox::draw() const
{
	ScrollWin::draw();

	if(sel >= 0) {
		bool hs = check_hscroll_vis();
		bool vs = check_vscroll_vis();
		IVec2 gpos = get_global_pos();

		gfx::push_clip();
		gfx::mult_clip(gpos.x, gpos.y, gpos.x + size.x - (vs ? vbar->get_size().x : 0), gpos.y + size.y - (hs ? hbar->get_size().y : 0));

		int i = 0;
		VBox::iterator iter = vbox->begin();
		while(iter != vbox->end()) {
			if(i++ == sel) {
				IVec2 gpos = (*iter)->get_global_pos();
				IVec2 sz = (*iter)->get_size();

				gfx::color_clamp(80, 120, 255, 64);
				gfx::rect(gpos.x, gpos.y, gpos.x + vbox->get_size().x, gpos.y + sz.y);
				break;
			}
			iter++;
		}

		gfx::pop_clip();
	}
}

ListBox *create_listbox(Widget *parent, int width, int height, Callback func, void *cdata)
{
	ListBox *lbox = new ListBox;
	lbox->set_callback(EVENT_MODIFY, func, cdata);
	lbox->set_size(width, height);
	parent->add_child(lbox);
	return lbox;
}

ListBox *create_listbox(Widget *parent, int width, int height, const char **items, int inum, Callback func, void *cdata)
{
	ListBox *lbox = create_listbox(parent, width, height, func, cdata);

	for(int i=0; i<inum; i++) {
		lbox->add_item(items[i]);
	}
	return lbox;
}

ListBox *create_listbox(Widget *parent, int width, int height, int *link)
{
	// TODO
	return 0;
}

ListBox *create_listbox(Widget *parent, int width, int height, const char *link)
{
	// TODO
	return 0;
}

void destroy_listbox(ListBox *lb)
{
	if(dynamic_cast<ListBox*>(lb)) {
		delete lb;
	} else {
		utk_error("non-listbox widget passed to destroy_listbox()\n");
	}
}


} // end of namespace utk
