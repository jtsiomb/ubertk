/*
ubertk is a flexible GUI toolkit targetted towards graphics applications.
Copyright (C) 2007 - 2008 John Tsiombikas <nuclear@member.fsf.org>,
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
#ifndef UBERTK_WIDGET_H_
#define UBERTK_WIDGET_H_

#include <utk_types.h>
#include <utk_events.h>

namespace utk {

class Event;

class Widget {
protected:
	char *name;
	IVec2 pos, size, min_size, max_size;
	int padding;

	Widget *child, *parent;
	bool visible;
	bool focused;
	bool hover;

	bool herod_mode;
	
	class PopupMenu *popup;

	CallbackClosure callbacks[EVENT_COUNT];

	bool *link_bool;
	int *link_int;
	float *link_flt;
	char *link_str;
	int link_str_width;

public:
	Widget();
	virtual ~Widget();
	
	virtual void set_name(const char *name);
	virtual const char *get_name() const;

	virtual Widget *find_widget(const char *name);
	virtual const Widget *find_widget(const char *name) const;

	virtual const char *class_name() const;

	virtual Widget *handle_event(Event *event);

	virtual void destroy_children(bool enable);

	virtual void set_pos(int x, int y);
	virtual void set_pos(IVec2 pos);
	virtual IVec2 get_pos() const;
	virtual IVec2 get_global_pos() const;

	virtual void set_size(int w, int h);
	virtual IVec2 get_size() const;
	virtual int get_width() const;
	virtual int get_height() const;
	
	virtual void set_min_size(int w, int h);
	virtual IVec2 get_min_size() const;
	virtual int get_min_width() const;
	virtual int get_min_height() const;
	virtual void set_max_size(int w, int h);
	virtual IVec2 get_max_size() const;
	virtual int get_max_width() const;
	virtual int get_max_height() const;

	virtual void set_padding(int pad);
	virtual int get_padding() const;

	virtual void show();
	virtual void hide();
	virtual bool is_visible() const;

	virtual void set_focus(bool f);
	virtual bool get_focus() const;

	virtual void add_child(Widget *w);
	virtual Widget *get_child();
	virtual Widget *get_child_at(int x, int y);
	virtual const Widget *get_child() const;
	virtual bool remove_child(Widget *w = 0);

	virtual void set_parent(Widget *w);
	virtual Widget *get_parent();
	virtual const Widget *get_parent() const;
	virtual class Window *get_window();

	virtual void rise();
	virtual void sink();
	
	virtual class PopupMenu *get_popup() const;
	virtual void set_popup(class PopupMenu *popup);
	virtual void show_popup(int x=-1, int y=-1);

	virtual bool hit_test(int x, int y) const;

	virtual void draw() const;

	virtual void set_link(bool *ptr);
	virtual void set_link(int *ptr);
	virtual void set_link(float *ptr);
	virtual void set_link(char *ptr, int width);

	virtual void set_callback(int event_type, Callback cbfunc, void *data = 0);
	virtual Callback get_callback(int event_type) const;
	virtual void *get_callback_data(int event_type) const;

	virtual void callback(Event *event, int event_type);

	virtual void on_click(Event *event);
	virtual void on_focus(Event *event);
	virtual void on_modify(Event *event);
	
	inline void set_hover(bool hover) { this->hover = hover; }
};

}

#endif	// UBERTK_WIDGET_H_
