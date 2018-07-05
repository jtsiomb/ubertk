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
#include <utk_config.h>
#include <string>
#include <utk_widget.h>
#include <utk_types.h>

#ifndef UTK_DRAWABLE_H_
#define UTK_DRAWABLE_H_

namespace utk {

class UTK_API Drawable : public Widget {
protected:
	Color color, light_color, dark_color;
	int border;
	std::string text;

public:
	Drawable();
	virtual ~Drawable();

	virtual void set_text(const char *text);
	virtual const char *get_text() const;

	virtual void set_color(int r, int g, int b, int a = 255);
	virtual void set_color(const Color &col);
	virtual const Color &get_color() const;

	virtual void set_alpha(int a);
	virtual int get_alpha() const;

	virtual void set_border(int border);
	virtual int get_border() const;
};

}

#endif	// UTK_DRAWABLE_H_
