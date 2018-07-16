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
#ifndef UTK_WIN_H_
#define UTK_WIN_H_

#include <utk_config.h>
#include <utk_widget.h>
#include <utk_drawable.h>

namespace utk {

class UTK_API Window : public Drawable {
protected:
	int tbar_height;
	bool shaded;
	bool modal;
	Widget *win_focus;

public:
	Window();
	virtual ~Window();

	virtual Widget *handle_event(Event *event);

	virtual void show();

	virtual void set_pos(int x, int y);
	virtual void set_pos(IVec2 pos);

	virtual void set_size(int w, int h);
	virtual void set_size(IVec2 sz);

	virtual IVec2 get_frame_pos() const;
	virtual IVec2 get_frame_global_pos() const;

	virtual int get_frame_width() const;
	virtual int get_frame_height() const;
	virtual IVec2 get_frame_size() const;

	virtual void rise();
	virtual void sink();

	virtual void set_win_focus(Widget *w);
	virtual Widget *get_win_focus();
	virtual const Widget *get_win_focus() const;

	virtual void draw() const;

	inline void set_modal(bool modal) { this->modal = modal; }

	friend class WinFrame;
	friend void destroy_window(Widget *w);
};


class UTK_API WinFrame : public Drawable {
protected:
	Color modal_col;
	Color unfocused_col;
	unsigned int last_click; // to detect double-clicks
	bool shaded;
	IVec2 orig_size;	// used when shaded

	void update_geometry();
	void set_shade(bool shade);

public:
	WinFrame(Widget *child);
	virtual ~WinFrame();

	virtual Widget *handle_event(Event *event);

	virtual bool is_visible() const;

	virtual void draw() const;

	friend class Window;
};


UTK_API Window *create_window(Widget *parent, int x, int y, int w, int h, const char *title);
UTK_API void destroy_window(Widget *w);

}	// end of namespace utk

#endif	// UTK_WIN_H_
