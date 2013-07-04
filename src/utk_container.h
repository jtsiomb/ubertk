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

#ifndef UTK_CONTAINER_H_
#define UTK_CONTAINER_H_

#include <list>
#include <stdlib.h>
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

	// XXX O(n)
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

#endif	// UTK_CONTAINER_H_
