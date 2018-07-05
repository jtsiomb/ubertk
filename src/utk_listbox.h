/*
ubertk is a flexible GUI toolkit targetted towards graphics applications.
Copyright (C) 2007 - 2018 John Tsiombikas <nuclear@member.fsf.org>,
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
#ifndef UTK_LISTBOX_H_
#define UTK_LISTBOX_H_

#include <utk_config.h>
#include <utk_widget.h>
#include <utk_drawable.h>
#include <utk_scrollwin.h>
#include <utk_container.h>

namespace utk {

class UTK_API ListBox : public ScrollWin {
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

UTK_API ListBox *create_listbox(Widget *parent, int width, int height, Callback func = 0, void *cdata = 0);
UTK_API ListBox *create_listbox(Widget *parent, int width, int height, const char **items, int inum, Callback func = 0, void *cdata = 0);
UTK_API ListBox *create_listbox(Widget *parent, int width, int height, int *link);
UTK_API ListBox *create_listbox(Widget *parent, int width, int height, const char *link);
UTK_API void destroy_listbox(ListBox *lb);

} // end namespace utk

#endif	// UTK_LISTBOX_H_
