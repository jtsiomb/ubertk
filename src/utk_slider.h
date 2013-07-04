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

#ifndef UTK_SLIDER_H_
#define UTK_SLIDER_H_

#include <utk_scrollbar.h>
#include <utk_events.h>

namespace utk {

class Slider : public Scrollbar {
protected:
	float start, end;
	int vis_decimal;
	bool show_value;

public:
	Slider();
	virtual ~Slider();

	virtual void set_min(float start);
	virtual void set_max(float end);
	virtual void set_range(float start, float end);
	virtual float get_min() const;
	virtual float get_max() const;

	virtual void set_vis_decimal(int count);
	virtual int get_vis_decimal() const;

	virtual void set_value(float val);
	virtual float get_value() const;
	virtual void operator=(float val);
	virtual operator float() const;

	virtual void draw() const;

	virtual void on_modify(Event *event);
};

Slider *create_slider(Widget *parent, float start, float end, Callback cb = 0, void *cdata = 0);
Slider *create_slider(Widget *parent, float start, float end, float *link);
Slider *create_slider(Widget *parent, float start, float end, int *link);
Slider *create_slider(Widget *parent, float start, float end, char *link, int bufsz);

}	// end namespace utk

#endif	// UTK_SLIDER_H_
