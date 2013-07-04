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
#ifndef UBERTK_H_
#define UBERTK_H_

#include <utk_events.h>
#include <utk_gfx.h>

#include <utk_widget.h>
#include <utk_win.h>
#include <utk_container.h>
#include <utk_popup.h>
#include <utk_label.h>
#include <utk_arrow.h>
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
#include <utk_filedialog.h>
#include <utk_align.h>
#include <utk_menus.h>
#include <utk_combobox.h>

#define UBERTK_PRINT_ROOT ((Widget*)-1)

namespace utk {

Container *init(int x, int y);
void close(Container *root = 0);
void draw(Container *root = 0);

void set_root_widget(Container *root);
Container *get_root_widget();

unsigned int get_msec();

void class_name(Widget *w);
void print_widget_tree(Widget *root = UBERTK_PRINT_ROOT);

void *get_user_data();
void set_user_data(void *ud);
}

#endif	// UBERTK_H_
