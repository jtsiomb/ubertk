#ifndef UTK_LISTBOX_H_
#define UTK_LISTBOX_H_

#include <utk_widget.h>
#include <utk_drawable.h>
#include <utk_scrollwin.h>
#include <utk_container.h>

namespace utk {

class ListBox : public ScrollWin {
protected:
	int sel;
	VBox *vbox;

public:
	ListBox();
	virtual ~ListBox();

	virtual Widget *handle_event(Event *event);

	virtual void add_item(Widget *item, int pos = -1);
	virtual void add_item(const char *str, int pos = -1);
	virtual void remove_item(int pos = -1);

	virtual void select(int pos);
	virtual void select(const char *str);
	virtual int get_selected() const;
	virtual const char *get_selected_text() const;

	virtual void set_spacing(int sp);
	virtual int get_spacing() const;

	virtual void clear();

	virtual void draw() const;
};

ListBox *create_listbox(Widget *parent, int width, int height, Callback func = 0, void *cdata = 0);
ListBox *create_listbox(Widget *parent, int width, int height, const char **items, int inum, Callback func = 0, void *cdata = 0);
ListBox *create_listbox(Widget *parent, int width, int height, int *link);
ListBox *create_listbox(Widget *parent, int width, int height, const char *link);
void destroy_listbox(ListBox *lb);

} // end namespace utk

#endif	// UTK_LISTBOX_H_
