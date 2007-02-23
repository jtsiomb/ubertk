#ifndef UBERTK_H_
#define UBERTK_H_

#include <utk_events.h>
#include <utk_gfx.h>

#include <utk_widget.h>
#include <utk_win.h>
#include <utk_container.h>
#include <utk_label.h>
#include <utk_button.h>
#include <utk_scrollbar.h>
#include <utk_entry.h>
#include <utk_chkbox.h>
#include <utk_radio.h>
#include <utk_img.h>
#include <utk_tball.h>
#include <utk_slider.h>
#include <utk_colorbox.h>
#include <utk_hsv.h>
#include <utk_progress.h>
#include <utk_huebox.h>
#include <utk_scrollwin.h>
#include <utk_listbox.h>
#include <utk_dialog.h>

namespace utk {

Container *init(int x, int y);
void close(Container *root = 0);
void draw(Container *root = 0);

Container *get_root_widget();

unsigned int get_msec();
	
}

#endif	// UBERTK_H_
