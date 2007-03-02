#include "ubertk.h"
#include "utk_gfx.h"

#if defined(unix) || defined(__unix__)
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
#if defined(__unix__) || defined(unix)
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
