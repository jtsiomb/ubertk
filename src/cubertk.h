/*
ubertk is a flexible GUI toolkit targetted towards graphics applications.
Copyright (C) 2007 - 2020 John Tsiombikas <nuclear@member.fsf.org>,
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
#ifndef CUBERTK_H_
#define CUBERTK_H_

#include "utk_config.h"

#ifndef CUBERTK_IMPL
typedef struct utk_widget utk_widget;
typedef struct utk_event utk_event;
#endif

enum {
	UTK_MOUSE_LEFT,
	UTK_MOUSE_MIDDLE,
	UTK_MOUSE_RIGHT,
	UTK_MOUSE_AUX1,
	UTK_MOUSE_AUX2
};
#define UTK_MOUSE_NONE	(-1)

/* these correspond to X11 keysyms */
enum {
	UTK_KEY_ESCAPE		= 27,
	UTK_KEY_DELETE		= 127,
	UTK_KEY_UP			= 273,
	UTK_KEY_DOWN		= 274,
	UTK_KEY_RIGHT		= 275,
	UTK_KEY_LEFT		= 276,
	UTK_KEY_INSERT		= 277,
	UTK_KEY_HOME		= 278,
	UTK_KEY_END			= 279,
	UTK_KEY_PAGEUP		= 280,
	UTK_KEY_PAGEDOWN	= 281
};

enum {
	UTK_EVENT_NULL,
	UTK_EVENT_MMOTION,
	UTK_EVENT_MBUTTON,
	UTK_EVENT_MHOVER,
	UTK_EVENT_KEYBOARD,
	UTK_EVENT_CLICK		= 100,
	UTK_EVENT_FOCUS,
	UTK_EVENT_MODIFY,
	UTK_EVENT_NOTIFY
};

enum { UTK_FOCUS_POINT, UTK_FOCUS_CLICK };

enum {
	UTK_MSG_BN_OK			= 0x01,
	UTK_MSG_BN_CANCEL		= 0x02,
	UTK_MSG_BN_OK_CANCEL	= 0x03,
	UTK_MSG_BN_YES			= 0x04,
	UTK_MSG_BN_NO			= 0x08,
	UTK_MSG_BN_YES_NO		= 0x0c
};
enum {
	UTK_MSG_TYPE_QUESTION,
	UTK_MSG_TYPE_INFO,
	UTK_MSG_TYPE_WARNING,
	UTK_MSG_TYPE_ERROR
};
enum { UTK_FILE_DIALOG_OPEN, UTK_FILE_DIALOG_SAVE };
enum { UTK_DLG_MODAL = 0x8000 };


typedef void (*utk_callback_func)(utk_event *ev, void *data);

typedef void (*utk_color_func)(int r, int g, int b, int a);
typedef void (*utk_clip_func)(int x1, int y1, int x2, int y2);
typedef void (*utk_image_func)(int x, int y, const void *pix, int xsz, int ysz);
typedef void (*utk_rect_func)(int x1, int y1, int x2, int y2);
typedef void (*utk_line_func)(int x1, int y1, int x2, int y2, int width);
typedef void (*utk_text_func)(int x, int y, const char *txt, int sz);
typedef int (*utk_text_spacing_func)(void);
typedef int (*utk_text_width_func)(const char *txt, int sz);

#define UTK_DEF_PADDING	0
#define UTK_DEF_SPACING	4

#ifdef __cplusplus
extern "C" {
#endif

UTK_API utk_widget *utk_init(int x, int y);
UTK_API void utk_close(utk_widget *root);
UTK_API void utk_draw(utk_widget *root);

UTK_API void utk_set_root_widget(utk_widget *root);
UTK_API utk_widget *utk_get_root_widget(void);

UTK_API unsigned int utk_get_msec(void);

UTK_API void utk_print_widget_tree(utk_widget *root);

UTK_API void *utk_get_user_data(void);
UTK_API void utk_set_user_data(void *ud);

UTK_API void utk_set_focus_mode(unsigned int fmode);
UTK_API unsigned int utk_get_focus_mode(void);

/* graphics callbacks */
UTK_API void utk_set_color_func(utk_color_func func);
UTK_API void utk_set_clip_func(utk_clip_func func);
UTK_API void utk_set_image_func(utk_image_func func);
UTK_API void utk_set_rect_func(utk_rect_func func);
UTK_API void utk_set_line_func(utk_line_func func);
UTK_API void utk_set_text_func(utk_text_func func);
UTK_API void utk_set_text_spacing_func(utk_text_spacing_func func);
UTK_API void utk_set_text_width_func(utk_text_width_func func);

/* low-level events, generated by the app */
UTK_API void utk_mmotion_event(int x, int y);
UTK_API void utk_mbutton_event(int bn, int press, int x, int y);
UTK_API void utk_keyboard_event(int key, int press);

/* UTK events */
UTK_API int utk_event_type(utk_event *ev);
UTK_API utk_widget *utk_event_widget(utk_event *ev);

/* widget */
UTK_API void utk_set_name(utk_widget *w, const char *name);
UTK_API const char *utk_get_name(utk_widget *w);
UTK_API utk_widget *utk_find_widget(utk_widget *w, const char *name);
UTK_API const char *utk_class_name(utk_widget *w);

UTK_API void utk_set_pos(utk_widget *w, int x, int y);
UTK_API void utk_get_pos(utk_widget *w, int *xp, int *yp);
UTK_API void utk_get_global_pos(utk_widget *w, int *xp, int *yp);
UTK_API void utk_set_size(utk_widget *w, int x, int y);
UTK_API void utk_get_size(utk_widget *w, int *xp, int *yp);
UTK_API int utk_get_width(utk_widget *w);
UTK_API int utk_get_height(utk_widget *w);
UTK_API void utk_set_min_size(utk_widget *w, int x, int y);
UTK_API void utk_get_min_size(utk_widget *w, int *xp, int *yp);
UTK_API int utk_get_min_width(utk_widget *w);
UTK_API int utk_get_min_height(utk_widget *w);
UTK_API void utk_set_max_size(utk_widget *w, int x, int y);
UTK_API void utk_get_max_size(utk_widget *w, int *xp, int *yp);
UTK_API int utk_get_max_width(utk_widget *w);
UTK_API int utk_get_max_height(utk_widget *w);

UTK_API void utk_set_padding(utk_widget *w, int pad);
UTK_API int utk_get_padding(utk_widget *w);

UTK_API void utk_show(utk_widget *w);
UTK_API void utk_hide(utk_widget *w);
UTK_API int utk_is_visible(utk_widget *w);

UTK_API void utk_set_focus(utk_widget *w, int onoff);
UTK_API int utk_get_focus(utk_widget *w);

UTK_API void utk_add_child(utk_widget *w, utk_widget *c);
UTK_API utk_widget *utk_get_child(utk_widget *w);
UTK_API utk_widget *utk_get_child_at(utk_widget *w, int x, int y);
UTK_API int utk_remove_child(utk_widget *w, utk_widget *c);

UTK_API void utk_set_parent(utk_widget *w, utk_widget *par);
UTK_API utk_widget *utk_get_parent(utk_widget *w);
UTK_API utk_widget *utk_get_window(utk_widget *w);

UTK_API void utk_rise(utk_widget *w);
UTK_API void utk_sink(utk_widget *w);

UTK_API utk_widget *utk_get_popup(utk_widget *w);
UTK_API void utk_set_popup(utk_widget *w, utk_widget *popup);
UTK_API void utk_show_popup(utk_widget *w, int x, int y);	/* def: -1 */

UTK_API int utk_hit_test(utk_widget *w, int x, int y);

UTK_API void utk_link_bool(utk_widget *w, int *ptr);
UTK_API void utk_link_int(utk_widget *w, int *ptr);
UTK_API void utk_link_float(utk_widget *w, float *ptr);
UTK_API void utk_link_str(utk_widget *w, char *ptr, int bufsz);

UTK_API void utk_set_callback(utk_widget *w, int evtype, utk_callback_func func, void *data);
UTK_API utk_callback_func utk_get_callback(utk_widget *w, int evtype);
UTK_API void *utk_get_callback_data(utk_widget *w, int evtype);

UTK_API void utk_set_widget_data(utk_widget *w, void *data);
UTK_API void *utk_get_widget_data(utk_widget *w);

/* drawable */
UTK_API void utk_set_color(utk_widget *w, int r, int g, int b);
UTK_API void utk_get_color(utk_widget *w, int *r, int *g, int *b);
UTK_API void utk_set_alpha(utk_widget *w, int a);
UTK_API int utk_get_alpha(utk_widget *w);
UTK_API void utk_set_border(utk_widget *w, int border);
UTK_API int utk_get_border(utk_widget *w);

/* window */
UTK_API utk_widget *utk_window(utk_widget *par, int x, int y, int w, int h, const char *title);
UTK_API void utk_destroy_window(utk_widget *w);

UTK_API void utk_get_frame_pos(utk_widget *w, int *x, int *y);
UTK_API void utk_get_frame_global_pos(utk_widget *w, int *x, int *y);
UTK_API void utk_get_frame_size(utk_widget *w, int *x, int *y);
UTK_API int utk_get_frame_width(utk_widget *w);
UTK_API int utk_get_frame_height(utk_widget *w);
UTK_API void utk_set_modal(utk_widget *w, int onoff);

/* container */
UTK_API utk_widget *utk_hbox(utk_widget *par, int pad, int spc);
UTK_API utk_widget *utk_vbox(utk_widget *par, int pad, int spc);
UTK_API utk_widget *utk_nullbox(utk_widget *par);

UTK_API void utk_raise_child(utk_widget *w, utk_widget *c);
UTK_API void utk_sink_child(utk_widget *w, utk_widget *c);
UTK_API void utk_set_spacing(utk_widget *w, int sp);
UTK_API int utk_get_spacing(utk_widget *w);

UTK_API utk_widget *utk_cont_child(utk_widget *w, int idx);
UTK_API int utk_cont_num_children(utk_widget *w);
UTK_API void utk_cont_clear(utk_widget *w);

/* label */
UTK_API utk_widget *utk_label(utk_widget *par, const char *txt);
UTK_API void utk_set_text_color(utk_widget *w, int r, int g, int b, int a);
UTK_API void utk_get_text_color(utk_widget *w, int *r, int *g, int *b, int *a);

/* button */
/* - button size is automatically calculated if xsz and/or ysz are 0
 * - the callback can be null
 */
UTK_API utk_widget *utk_button(utk_widget *par, const char *txt, int xsz, int ysz,
		utk_callback_func cb, void *cbdata);
UTK_API utk_widget *utk_widget_button(utk_widget *par, utk_widget *child, int xsz,
		int ysz, utk_callback_func cb, void *cbdata);

UTK_API void utk_set_flat(utk_widget *w, int onoff);
UTK_API int utk_is_flat(utk_widget *w);

/* checkbox */
UTK_API utk_widget *utk_checkbox(utk_widget *par, const char *txt, int checked,
		utk_callback_func cb, void *cdata);
UTK_API utk_widget *utk_checkbox_link(utk_widget *par, const char *txt, int checked,
		int *link);

UTK_API void utk_check(utk_widget *w);
UTK_API void utk_uncheck(utk_widget *w);
UTK_API void utk_set_checked(utk_widget *w, int onoff);
UTK_API int utk_is_checked(utk_widget *w);

/* radiobox */
UTK_API utk_widget *utk_radiobox(utk_widget *par, const char *txt, int checked,
		utk_callback_func cb, void *cdata);
UTK_API utk_widget *utk_radiobox_link(utk_widget *par, const char *txt, int checked,
		int *link);

/* listbox */
UTK_API utk_widget *utk_listbox(utk_widget *par, int w, int h, utk_callback_func cb, void *cdata);
UTK_API utk_widget *utk_listbox_items(utk_widget *par, int w, int h, const char **items,
		int num_items, utk_callback_func cb, void *cdata);
UTK_API utk_widget *utk_listbox_linkint(utk_widget *par, int w, int h, int *link);
UTK_API utk_widget *utk_listbox_linkstr(utk_widget *par, int w, int h, const char *link);

UTK_API void utk_add_item(utk_widget *w, utk_widget *item, int pos); /* -1 appends */
UTK_API void utk_add_string(utk_widget *w, const char *str, int pos);  /* ditto */
/*UTK_API void utk_remove_item(utk_widget *w, int pos);*/ /* -1 removes last */

UTK_API void utk_select(utk_widget *w, int pos);
UTK_API void utk_select_name(utk_widget *w, const char *str);
UTK_API int utk_get_selected(utk_widget *w);
UTK_API const char *utk_get_selected_str(utk_widget *w);

/* combo box */
UTK_API utk_widget *utk_combobox(utk_widget *par, utk_callback_func cb, void *cdata);
UTK_API utk_widget *utk_combobox_items(utk_widget *par, const char **items,
		int num_items, utk_callback_func cb, void *cdata);
UTK_API utk_widget *utk_combobox_linkint(utk_widget *par, int *link);
UTK_API utk_widget *utk_combobox_linkstr(utk_widget *par, const char *link);

UTK_API void utk_show_list(utk_widget *w);
UTK_API void utk_set_readonly(utk_widget *w, int onoff);
UTK_API int utk_is_readonly(utk_widget *w);

/* dialogs */
UTK_API utk_widget *utk_message_dialog(const char *msg, unsigned int type,
		unsigned int bnmask, utk_callback_func func, void *cdata);
UTK_API utk_widget *utk_input_dialog(const char *msg, const char *title,
		const char *deftext, utk_callback_func func, void *data);
UTK_API utk_widget *utk_file_dialog(unsigned int type, const char *fname,
		const char *filter, const char *initdir, utk_callback_func func, void *cdata);
UTK_API int utk_is_dialog(utk_widget *w);

UTK_API const char *utk_file_dialog_dir(utk_widget *w);
UTK_API const char *utk_file_dialog_file(utk_widget *w);
UTK_API const char *utk_file_dialog_path(utk_widget *w);

#ifdef __cplusplus
}
#endif

#endif	/* CUBERTK_H_ */
