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
#include "ubertk.h"
#include "utk_gfx.h"

#if defined(unix) || defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#include <time.h>
#include <sys/time.h>
#else 
#include <windows.h>
#endif	/* unix */

namespace utk {

#include "cursor.inl"

static Container *root_widget;
static bool draw_cursor;

Container *init(int x, int y)
{
	NullBox *cont = new NullBox;
	cont->set_size(x, y);
	cont->set_pos(0, 0);
	root_widget = cont;

#if defined(WIN32) || defined(__WIN32__)
	draw_cursor = true;
#endif

	get_msec();	// initialize timer
	
	return cont;
}

void close(Container *root)
{
	if(!root) root = root_widget;
	delete root;
}

void draw(Container *root)
{
	if(!root) root = root_widget;

	IVec2 cpos, csz;
	cpos = root->get_pos();
	csz = root->get_size();

	gfx::push_clip();
	gfx::set_clip(cpos.x, cpos.y, cpos.x + csz.x, cpos.y + csz.y);
	root->draw();
	
	if(draw_cursor) {
		gfx::image(get_mouse_pos().x, get_mouse_pos().y, utk_cursor_image, 16, 16);
	}

	gfx::pop_clip();
}

Container *get_root_widget()
{
	return root_widget;
}

unsigned int get_msec()
{
#if defined(__unix__) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
	static struct timeval timeval, first_timeval;
	
	gettimeofday(&timeval, 0);

	if(first_timeval.tv_sec == 0) {
		first_timeval = timeval;
		return 0;
	}
	return (timeval.tv_sec - first_timeval.tv_sec) * 1000 + (timeval.tv_usec - first_timeval.tv_usec) / 1000;
#else
	return GetTickCount();
#endif	/* __unix__ */
}

void print_widget_tree(Widget *w)
{
	static int lvl;
	if(!w) return;
	if(w == (Widget*)0xffffffff) w = root_widget;

	for(int i=0; i<lvl; i++) {
		fputs("  ", stdout);
	}
	fputs(w->class_name(), stdout);

	Drawable *draw;
	if((draw = dynamic_cast<Drawable*>(w))) {
		const char *text = draw->get_text();

		if(text && *text) {
			printf("  (%s)", text);
		}
	}

	printf("   [%dx%d-%d+%d]", w->get_size().x, w->get_size().y, w->get_pos().x, w->get_pos().y);
	putchar('\n');

	lvl++;

	Container *cont;
	if((cont = dynamic_cast<Container*>(w))) {
		Container::iterator iter = cont->begin();
		while(iter != cont->end()) {
			print_widget_tree(*iter++);
		}
	} else {
		print_widget_tree(w->get_child());
	}

	lvl--;
}

}	// namespace utk end
