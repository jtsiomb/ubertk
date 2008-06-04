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
#ifndef UBERTK_BUTTON_H_
#define UBERTK_BUTTON_H_

#include <utk_drawable.h>
#include <utk_events.h>

namespace utk {

class Button : public Drawable {
protected:
	bool pressed;
	bool flat;

public:
	Button(Widget *widget, Callback cb = 0);
	virtual ~Button();

	virtual Widget *handle_event(Event *event);

	virtual Widget *get_child_at(int x, int y);
	virtual void set_size(int w, int h);

	virtual void draw() const;
	
	virtual void set_text(const char *text);

	virtual void set_flat(bool flat);
	virtual bool is_flat() const;
};

Button *create_button(Widget *parent, const char *text, Callback func = 0, void *cdata = 0);
Button *create_button(Widget *parent, const char *text, int xsz, int ysz = 0, Callback func = 0, void *cdata = 0);
Button *create_button(Widget *parent, Widget *child, Callback func = 0, void *cdata = 0);
Button *create_button(Widget *parent, Widget *child, int xsz, int ysz = 0, Callback func = 0, void *cdata = 0);

}	// end of namespace utk

#endif	// UBERTK_BUTTON_H_

