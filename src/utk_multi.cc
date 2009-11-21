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

// utk_multi.cc

#include "utk_multi.h"
#include <assert.h>

using namespace utk;

static UTKContext *get_context()
{
	UTKContext *context = (UTKContext*)get_user_data();
	assert(context);
	return context;
}

static void multi_color_func(int r, int g, int b, int a)
{
	get_context()->color(r, g, b, a);
}

static void multi_clip_func(int x1, int y1, int x2, int y2)
{
	get_context()->clip(x1, y1, x2, y2);
}

static void multi_image_func(int x, int y, const void *pix, int xsz, int ysz)
{
	get_context()->image(x, y, pix, xsz, ysz);
}

static void multi_rect_func(int x1, int y1, int x2, int y2)
{
	get_context()->rect(x1, y1, x2, y2);
}

static void multi_line_func(int x1, int y1, int x2, int y2, int width)
{
	get_context()->line(x1, y1, x2, y2, width);
}

static void multi_text_func(int x, int y, const char *txt, int sz)
{
	get_context()->text(x, y, txt, sz);
}

static int multi_text_spacing_func()
{
	return get_context()->text_spacing();
}

static int multi_text_width_func(const char *txt, int sz)
{
	return get_context()->text_width(txt, sz);
}


void UTKContext::begin()
{
	// set the user pointer to point to us
	old_user_data = get_user_data();
	set_user_data(this);

	// set the current root widget to our root
	old_root = get_root_widget();
	set_root_widget(root);

	// set multi* drawing functions
	old_color_func = gfx::color;
	gfx::color = multi_color_func;

	old_clip_func = gfx::clip;
	gfx::clip = multi_clip_func;

	old_image_func = gfx::image;
	gfx::image = multi_image_func;

	old_rect_func = gfx::rect;
	gfx::rect = multi_rect_func;

	old_line_func = gfx::line;
	gfx::line = multi_line_func;

	old_text_func = gfx::text;
	gfx::text = multi_text_func;

	old_text_spacing_func = gfx::text_spacing;
	gfx::text_spacing = multi_text_spacing_func;

	old_text_width_func = gfx::text_width;
	gfx::text_width = multi_text_width_func;
}

void UTKContext::end()
{
	// reset old drawing functions
	old_root = get_root_widget();
	set_root_widget(root);

	// set multi* drawing functions
	gfx::color = old_color_func;
	gfx::clip = old_clip_func;
	gfx::image = old_image_func;
	gfx::rect = old_rect_func;
	gfx::line = old_line_func;
	gfx::text = old_text_func;
	gfx::text_spacing = old_text_spacing_func;
	gfx::text_width = old_text_width_func;

	// reset old root widget
	set_root_widget(old_root);

	// reset old user pointer
	set_user_data(old_user_data);
}

UTKContext::UTKContext(int w, int h)
{
	root = init(w, h);
	assert(root);
}

UTKContext::~UTKContext()
{
	close(root);
	root = NULL;
}

void UTKContext::draw()
{
	begin();
	utk::draw(root);
	end();
}

