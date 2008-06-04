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
#ifndef UBERTK_SCROLLBAR_H_
#define UBERTK_SCROLLBAR_H_

#include <utk_drawable.h>
#include <utk_events.h>

namespace utk {

class Scrollbar : public Drawable {
protected:
	int cursor_width;
	int track_len, track_start;
	int cursor_pos;

	bool dragging;
	int orient;

	IVec2 get_cursor_tl() const;
	IVec2 get_cursor_br() const;

public:
	Scrollbar();
	virtual ~Scrollbar();

	virtual Widget *handle_event(Event *event);

	virtual void set_size(int w, int h);
	virtual void set_border(int border);

	virtual void set_orientation(int orient);
	virtual int get_orientation() const;

	virtual void set_cursor_width(int width);
	virtual void set_cursor_width(float percent);
	virtual int get_cursor_width() const;
	
	virtual void set_value(float val);
	virtual float get_value() const;
	virtual void operator=(float val);
	virtual operator float() const;

	virtual void draw() const;
};

}

#endif // ndef UBERTK_SCROLLBAR_H_
