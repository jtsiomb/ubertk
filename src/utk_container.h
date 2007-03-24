#ifndef UBERTK_CONTAINER_H_
#define UBERTK_CONTAINER_H_

#include <list>
#include <utk_widget.h>

namespace utk {

class Container : public Widget {
private:
	mutable std::list<Widget*>::iterator cache_iter;
	mutable int cache_idx;

	std::list<Widget*>::iterator get_iter_at(int idx) const;

protected:
	std::list<Widget*> cont;
	int spacing;

public:
	typedef std::list<Widget*>::iterator iterator;
	typedef std::list<Widget*>::const_iterator const_iterator;

	Container();
	virtual ~Container();

	virtual const Widget *find_widget(const char *name) const;

	virtual void layout();

	virtual void add_child(Widget *w);
	virtual bool remove_child(Widget *w);
	virtual Widget *get_child_at(int x, int y);

	virtual void raise_child(Widget *w);
	virtual void sink_child(Widget *w);

	virtual void set_spacing(int sp);
	virtual int get_spacing() const;

	virtual iterator begin();
	virtual const_iterator begin() const;
	virtual iterator end();
	virtual const_iterator end() const;

	virtual Widget *operator[](int idx);
	virtual const Widget *operator[](int idx) const;

	virtual void clear();

	virtual size_t size() const;
	virtual bool empty() const;

	virtual void draw() const;	// just calls the draw() of each contained widget
};



class HBox : public Container {
public:
	virtual void layout();
};

class VBox : public Container {
public:
	virtual void layout();
};

class NullBox : public Container {
public:
	virtual void layout();
};


HBox *create_hbox(Widget *parent, int padding = 0, int spacing = 4);
VBox *create_vbox(Widget *parent, int padding = 0, int spacing = 4);
NullBox *create_nullbox(Widget *parent);

}	// end of namespace utk

#endif	// UBERTK_CONTAINER_H_
