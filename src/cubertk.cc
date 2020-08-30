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
#include <assert.h>
#include "ubertk.h"

#define CUBERTK_IMPL
typedef utk::Widget utk_widget;
typedef utk::Event utk_event;
#include "cubertk.h"

#define UTK_DRAWABLE(w)	(assert(dynamic_cast<utk::Drawable*>(w)), (utk::Drawable*)(w))
#define UTK_WINDOW(w)	(assert(dynamic_cast<utk::Window*>(w)), (utk::Window*)(w))
#define UTK_CONTAINER(w) (assert(dynamic_cast<utk::Container*>(w)), (utk::Container*)(w))
#define UTK_LABEL(w)	(assert(dynamic_cast<utk::Label*>(w)), (utk::Label*)(w))

extern "C" {

utk_widget *utk_init(int x, int y)
{
	return utk::init(x, y);
}

void utk_close(utk_widget *root)
{
	assert(dynamic_cast<utk::Container*>(root));
	utk::close((utk::Container*)root);
}

void utk_draw(utk_widget *root)
{
	assert(dynamic_cast<utk::Container*>(root));
	utk::draw((utk::Container*)root);
}

void utk_set_root_widget(utk_widget *root)
{
	assert(dynamic_cast<utk::Container*>(root));
	utk::set_root_widget((utk::Container*)root);
}

utk_widget *utk_get_root_widget(void)
{
	return utk::get_root_widget();
}

unsigned int utk_get_msec(void)
{
	return utk::get_msec();
}

void utk_print_widget_tree(utk_widget *root)
{
	utk::print_widget_tree(root);
}

void *utk_get_user_data(void)
{
	return utk::get_user_data();
}

void utk_set_user_data(void *ud)
{
	utk::set_user_data(ud);
}

void utk_set_color_func(utk_color_func func)
{
	utk::gfx::color = func;
}

void utk_set_clip_func(utk_clip_func func)
{
	utk::gfx::clip = func;
}

void utk_set_image_func(utk_image_func func)
{
	utk::gfx::image = func;
}

void utk_set_rect_func(utk_rect_func func)
{
	utk::gfx::rect = func;
}

void utk_set_line_func(utk_line_func func)
{
	utk::gfx::line = func;
}

void utk_set_text_func(utk_text_func func)
{
	utk::gfx::text = func;
}

void utk_set_text_spacing_func(utk_text_spacing_func func)
{
	utk::gfx::text_spacing = func;
}

void utk_set_text_width_func(utk_text_width_func func)
{
	utk::gfx::text_width = func;
}

/* ---- widget ---- */
void utk_set_name(utk_widget *w, const char *name)
{
	w->set_name(name);
}

const char *utk_get_name(utk_widget *w)
{
	return w->get_name();
}

utk_widget *utk_find_widget(utk_widget *w, const char *name)
{
	return w->find_widget(name);
}

const char *utk_class_name(utk_widget *w)
{
	return w->class_name();
}

void utk_set_pos(utk_widget *w, int x, int y)
{
	w->set_pos(x, y);
}

void utk_get_pos(utk_widget *w, int *xp, int *yp)
{
	utk::IVec2 pos = w->get_pos();
	*xp = pos.x;
	*yp = pos.y;
}

void utk_get_global_pos(utk_widget *w, int *xp, int *yp)
{
	utk::IVec2 pos = w->get_global_pos();
	*xp = pos.x;
	*yp = pos.y;
}

void utk_set_size(utk_widget *w, int x, int y)
{
	w->set_size(x, y);
}

void utk_get_size(utk_widget *w, int *xp, int *yp)
{
	utk::IVec2 sz = w->get_size();
	*xp = sz.x;
	*yp = sz.y;
}

int utk_get_width(utk_widget *w)
{
	return w->get_size().x;
}

int utk_get_height(utk_widget *w)
{
	return w->get_size().y;
}

void utk_set_min_size(utk_widget *w, int x, int y)
{
	w->set_min_size(x, y);
}

void utk_get_min_size(utk_widget *w, int *xp, int *yp)
{
	utk::IVec2 sz = w->get_min_size();
	*xp = sz.x;
	*yp = sz.y;
}

int utk_get_min_width(utk_widget *w)
{
	return w->get_min_size().x;
}

int utk_get_min_height(utk_widget *w)
{
	return w->get_min_size().y;
}

void utk_set_max_size(utk_widget *w, int x, int y)
{
	w->set_max_size(x, y);
}

void utk_get_max_size(utk_widget *w, int *xp, int *yp)
{
	utk::IVec2 sz = w->get_max_size();
	*xp = sz.x;
	*yp = sz.y;
}

int utk_get_max_width(utk_widget *w)
{
	return w->get_max_size().x;
}

int utk_get_max_height(utk_widget *w)
{
	return w->get_max_size().y;
}

void utk_set_padding(utk_widget *w, int pad)
{
	w->set_padding(pad);
}

int utk_get_padding(utk_widget *w)
{
	return w->get_padding();
}

void utk_show(utk_widget *w)
{
	w->show();
}

void utk_hide(utk_widget *w)
{
	w->hide();
}

int utk_is_visible(utk_widget *w)
{
	return w->is_visible();
}

void utk_set_focus(utk_widget *w, int onoff)
{
	w->set_focus(onoff);
}

int utk_get_focus(utk_widget *w)
{
	return w->get_focus() ? 1 : 0;
}

void utk_add_child(utk_widget *w, utk_widget *c)
{
	w->add_child(c);
}

utk_widget *utk_get_child(utk_widget *w)
{
	return w->get_child();
}

utk_widget *utk_get_child_at(utk_widget *w, int x, int y)
{
	return w->get_child_at(x, y);
}

int utk_remove_child(utk_widget *w, utk_widget *c)
{
	return w->remove_child(c) ? 0 : -1;
}

void utk_set_parent(utk_widget *w, utk_widget *par)
{
	w->set_parent(par);
}

utk_widget *utk_get_parent(utk_widget *w)
{
	return w->get_parent();
}

utk_widget *utk_get_window(utk_widget *w)
{
	return w->get_window();
}

void utk_rise(utk_widget *w)
{
	w->rise();
}

void utk_sink(utk_widget *w)
{
	w->sink();
}

utk_widget *utk_get_popup(utk_widget *w)
{
	return w->get_popup();
}

void utk_set_popup(utk_widget *w, utk_widget *popup)
{
	assert(dynamic_cast<utk::PopupMenu*>(popup));
	w->set_popup((utk::PopupMenu*)popup);
}

void utk_show_popup(utk_widget *w, int x, int y)
{
	w->show_popup(x, y);
}

int utk_hit_test(utk_widget *w, int x, int y)
{
	return w->hit_test(x, y);
}

void utk_link_bool(utk_widget *w, int *ptr)
{
	/* XXX dodgy */
	w->set_link((bool*)ptr);
}

void utk_link_int(utk_widget *w, int *ptr)
{
	w->set_link(ptr);
}

void utk_link_float(utk_widget *w, float *ptr)
{
	w->set_link(ptr);
}

void utk_link_str(utk_widget *w, char *ptr, int bufsz)
{
	w->set_link(ptr, bufsz);
}

void utk_set_callback(utk_widget *w, int evtype, utk_callback_func func, void *data)
{
	w->set_callback(evtype, func, data);
}

utk_callback_func utk_get_callback(utk_widget *w, int evtype)
{
	return w->get_callback(evtype);
}

void *utk_get_callback_data(utk_widget *w, int evtype)
{
	return w->get_callback_data(evtype);
}

void utk_set_widget_data(utk_widget *w, void *data)
{
	w->set_user_data(data);
}

void *utk_get_widget_data(utk_widget *w)
{
	return w->get_user_data();
}

/* drawable */
void utk_set_color(utk_widget *w, int r, int g, int b)
{
	utk::Drawable *dw = UTK_DRAWABLE(w);
	dw->set_color(r, g, b, dw->get_alpha());
}

void utk_get_color(utk_widget *w, int *r, int *g, int *b)
{
	utk::Drawable *dw = UTK_DRAWABLE(w);
	const utk::Color &col = dw->get_color();
	*r = col.r;
	*g = col.g;
	*b = col.b;
}

void utk_set_alpha(utk_widget *w, int a)
{
	utk::Drawable *dw = UTK_DRAWABLE(w);
	dw->set_alpha(a);
}

int utk_get_alpha(utk_widget *w)
{
	utk::Drawable *dw = UTK_DRAWABLE(w);
	return dw->get_alpha();
}

void utk_set_border(utk_widget *w, int border)
{
	utk::Drawable *dw = UTK_DRAWABLE(w);
	dw->set_border(border);
}

int utk_get_border(utk_widget *w)
{
	utk::Drawable *dw = UTK_DRAWABLE(w);
	return dw->get_border();
}

/* window */
utk_widget *utk_create_window(utk_widget *par, int x, int y, int w, int h, const char *title)
{
	return utk::create_window(par, x, y, w, h, title);
}

void utk_destroy_window(utk_widget *w)
{
	utk::destroy_window(w);
}

void utk_get_frame_pos(utk_widget *w, int *x, int *y)
{
	utk::Window *win = UTK_WINDOW(w);
	utk::IVec2 pos = win->get_frame_pos();
	*x = pos.x;
	*y = pos.y;
}

void utk_get_frame_global_pos(utk_widget *w, int *x, int *y)
{
	utk::Window *win = UTK_WINDOW(w);
	utk::IVec2 pos = win->get_frame_global_pos();
	*x = pos.x;
	*y = pos.y;
}

void utk_get_frame_size(utk_widget *w, int *x, int *y)
{
	utk::Window *win = UTK_WINDOW(w);
	utk::IVec2 sz = win->get_frame_size();
	*x = sz.x;
	*y = sz.y;
}

int utk_get_frame_width(utk_widget *w)
{
	utk::Window *win = UTK_WINDOW(w);
	return win->get_frame_width();
}

int utk_get_frame_height(utk_widget *w)
{
	utk::Window *win = UTK_WINDOW(w);
	return win->get_frame_height();
}

void utk_set_modal(utk_widget *w, int onoff)
{
	utk::Window *win = UTK_WINDOW(w);
	win->set_modal(onoff);
}

/* container */
utk_widget *utk_create_hbox(utk_widget *par, int pad, int spc)
{
	return utk::create_hbox(par, pad, spc);
}

utk_widget *utk_create_vbox(utk_widget *par, int pad, int spc)
{
	return utk::create_vbox(par, pad, spc);
}

utk_widget *utk_create_nullbox(utk_widget *par)
{
	return utk::create_nullbox(par);
}

void utk_raise_child(utk_widget *w, utk_widget *c)
{
	utk::Container *cont = UTK_CONTAINER(w);
	cont->raise_child(c);
}

void utk_sink_child(utk_widget *w, utk_widget *c)
{
	utk::Container *cont = UTK_CONTAINER(w);
	cont->sink_child(c);
}

void utk_set_spacing(utk_widget *w, int sp)
{
	utk::Container *cont = UTK_CONTAINER(w);
	cont->set_spacing(sp);
}

int utk_get_spacing(utk_widget *w)
{
	utk::Container *cont = UTK_CONTAINER(w);
	return cont->get_spacing();
}

utk_widget *utk_cont_child(utk_widget *w, int idx)
{
	utk::Container *cont = UTK_CONTAINER(w);
	return (*cont)[idx];
}

int utk_cont_num_children(utk_widget *w)
{
	utk::Container *cont = UTK_CONTAINER(w);
	return cont->size();
}

void utk_cont_clear(utk_widget *w)
{
	utk::Container *cont = UTK_CONTAINER(w);
	cont->clear();
}


/* label */
utk_widget *utk_create_label(utk_widget *par, const char *txt)
{
	return utk::create_label(par, txt);
}

void utk_set_text_color(utk_widget *w, int r, int g, int b, int a)
{
	utk::Label *lb = UTK_LABEL(w);
	lb->set_text_color(r, g, b, a);
}

void utk_get_text_color(utk_widget *w, int *r, int *g, int *b, int *a)
{
	utk::Label *lb = UTK_LABEL(w);
	const utk::Color &col = lb->get_text_color();
	*r = col.r;
	*g = col.g;
	*b = col.b;
	*a = col.a;
}

/* button */
utk_widget *utk_create_button(utk_widget *par, const char *txt, int xsz, int ysz,
		utk_callback_func cb, void *cbdata)
{
	if(xsz) {
		return utk::create_button(par, txt, xsz, ysz, cb, cbdata);
	}
	return utk::create_button(par, txt, cb, cbdata);
}

utk_widget *utk_create_widget_button(utk_widget *par, utk_widget *child, int xsz,
		int ysz, utk_callback_func cb, void *cbdata)
{
	if(xsz) {
		return utk::create_button(par, child, xsz, ysz, cb, cbdata);
	}
	return utk::create_button(par, child, cb, cbdata);
}

void utk_set_flat(utk_widget *w, int onoff)
{
	utk::Button *bn = dynamic_cast<utk::Button*>(w);
	if(bn) {
		bn->set_flat(onoff);
	}
}

int utk_is_flat(utk_widget *w)
{
	utk::Button *bn = dynamic_cast<utk::Button*>(w);
	if(bn) {
		return bn->is_flat() ? 1 : 0;
	}
	return 0;
}


}	// extern "C"
