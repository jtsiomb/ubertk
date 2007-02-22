#include "utk_listbox.h"
#include "utk_label.h"

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

void ListBox::draw() const
{
	ScrollWin::draw();
}

} // end of namespace utk
