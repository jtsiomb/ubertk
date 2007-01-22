#ifndef UBERTK_H_
#define UBERTK_H_

#include <utk_events.h>
#include <utk_gfx.h>

#include <utk_widget.h>
#include <utk_win.h>
#include <utk_container.h>
#include <utk_label.h>

namespace utk {

Container *init(int x, int y);
void close(Container *root = 0);
void draw(Container *root = 0);

Container *get_root_widget();
	
}

#endif	// UBERTK_H_
