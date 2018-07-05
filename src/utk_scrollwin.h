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
#ifndef UTK_SCROLLWIN_H_
#define UTK_SCROLLWIN_H_

#include <utk_config.h>
#include <utk_widget.h>
#include <utk_drawable.h>
#include <utk_scrollbar.h>

namespace utk {

enum {
	SCROLLBAR_HORIZONTAL	= 1,
	SCROLLBAR_VERTICAL		= 2,
	SCROLLBAR_BOTH			= 3
};
enum {
	SCROLLBAR_HIDE,
	SCROLLBAR_SHOW,
	SCROLLBAR_AUTO
};

class UTK_API ScrollWin : public Drawable {
protected:
	Scrollbar *hbar, *vbar;
	int sbar_width;

	int vbar_state, hbar_state;

	bool dragging;

	bool check_vscroll_vis() const;
	bool check_hscroll_vis() const;

public:
	bool ignore_bars;

	ScrollWin();
	virtual ~ScrollWin();

	virtual Widget *handle_event(Event *event);
	virtual Widget *get_child_at(int x, int y);

	virtual void set_size(int w, int h);
	virtual void set_size(IVec2 sz);

	virtual void set_scrollbar_state(unsigned int which, int state);
	virtual int get_scrollbar_state(unsigned int which);

	virtual void set_hscroll(float val);
	virtual void set_vscroll(float val);
	virtual float get_hscroll() const;
	virtual float get_vscroll() const;

	virtual void draw() const;
};

}

#endif	/* UTK_SCROLLWIN_H_ */
