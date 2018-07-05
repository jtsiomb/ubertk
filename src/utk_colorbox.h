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
#ifndef _UTK_COLORBOX_H_
#define _UTK_COLORBOX_H_

#include <utk_config.h>
#include <utk_img.h>

namespace utk {

class UTK_API ColorBox : public Image {
protected:
	float h, s, v;
	int sel_s, sel_v;

	virtual void update();
	virtual void update_cursors();

	virtual void on_click(Event *ev);
	virtual void on_modify(Event *ev);
	//virtual void on_motion(int x, int y);
	virtual void on_drag(int dx, int dy);
public:
	ColorBox(utk::Callback cb = 0);
	virtual ~ColorBox();

	float get_h() const;
	float get_s() const;
	float get_v() const;

	void set_h(float h);

	virtual void set_color(int r, int g, int b, int a = 255);
	virtual void set_color_hsv(float h, float s, float v, int a = 255);
	virtual void set_color(const Color &col);
	virtual unsigned int get_packed_color() const;
};

UTK_API ColorBox *create_colorbox(Widget *parent, Callback func = 0, void *cdata = 0);

}

#endif // ndef _UTK_COLORBOX_H_
