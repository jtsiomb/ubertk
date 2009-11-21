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
#include "utk_config.h"
#include <stdlib.h>
#include <string.h>
#include "utk_drawable.h"

namespace utk {

Drawable::Drawable()
{
	border = 0;
}

Drawable::~Drawable() {}

void Drawable::set_text(const char *str)
{
	text = std::string(str ? str : "");
}

const char *Drawable::get_text() const
{
	return text.c_str();
}
	

void Drawable::set_color(int r, int g, int b, int a)
{
	color = Color(r, g, b, a);
	light_color = lighter_color(color);
	dark_color = darker_color(color);
}

#define MIN(a, b)	((a) < (b) ? (a) : (b))
void Drawable::set_color(const Color &col)
{
	color = col;
	light_color = lighter_color(col);
	dark_color = darker_color(col);
}

const Color &Drawable::get_color() const
{
	return color;
}

void Drawable::set_alpha(int a)
{
	color.a = light_color.a = dark_color.a = a;
}

int Drawable::get_alpha() const
{
	return color.a;
}

void Drawable::set_border(int border)
{
	this->border = border;
}

int Drawable::get_border() const
{
	return border;
}

}	// end of utk namespace
