#include <assert.h>
#include "ubertk.h"

#define CUBERTK_IMPL
typedef utk::Widget utk_widget;
typedef utk::Event utk_event;
#include "cubertk.h"

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
UTK_API void utk_set_name(utk_widget *w, const char *name)
{
	w->set_name(name);
}

UTK_API const char *utk_get_name(utk_widget *w)
{
	return w->get_name();
}

UTK_API utk_widget *utk_find_widget(utk_widget *w, const char *name)
{
	return w->find_widget(name);
}

UTK_API const char *utk_class_name(utk_widget *w)
{
	return w->class_name();
}

UTK_API void utk_set_pos(utk_widget *w, int x, int y)
{
	w->set_pos(x, y);
}

UTK_API void utk_get_pos(utk_widget *w, int *xp, int *yp)
{
	utk::IVec2 pos = w->get_pos();
	*xp = pos.x;
	*yp = pos.y;
}

UTK_API void utk_get_global_pos(utk_widget *w, int *xp, int *yp)
{
	utk::IVec2 pos = w->get_global_pos();
	*xp = pos.x;
	*yp = pos.y;
}

UTK_API void utk_set_size(utk_widget *w, int x, int y)
{
	w->set_size(x, y);
}

UTK_API void utk_get_size(utk_widget *w, int *xp, int *yp)
{
	utk::IVec2 sz = w->get_size();
	*xp = sz.x;
	*yp = sz.y;
}

UTK_API int utk_get_width(utk_widget *w)
{
	return w->get_size().x;
}

UTK_API int utk_get_height(utk_widget *w)
{
	return w->get_size().y;
}

UTK_API void utk_set_min_size(utk_widget *w, int x, int y)
{
	w->set_min_size(x, y);
}

UTK_API void utk_get_min_size(utk_widget *w, int *xp, int *yp)
{
	utk::IVec2 sz = w->get_min_size();
	*xp = sz.x;
	*yp = sz.y;
}

UTK_API int utk_get_min_width(utk_widget *w)
{
	return w->get_min_size().x;
}

UTK_API int utk_get_min_height(utk_widget *w)
{
	return w->get_min_size().y;
}

UTK_API void utk_set_max_size(utk_widget *w, int x, int y)
{
	w->set_max_size(x, y);
}

UTK_API void utk_get_max_size(utk_widget *w, int *xp, int *yp)
{
	utk::IVec2 sz = w->get_max_size();
	*xp = sz.x;
	*yp = sz.y;
}

UTK_API int utk_get_max_width(utk_widget *w)
{
	return w->get_max_size().x;
}

UTK_API int utk_get_max_height(utk_widget *w)
{
	return w->get_max_size().y;
}

UTK_API void utk_set_padding(utk_widget *w, int pad)
{
	w->set_padding(pad);
}

UTK_API int utk_get_padding(utk_widget *w)
{
	return w->get_padding();
}

UTK_API void utk_show(utk_widget *w)
{
	w->show();
}

UTK_API void utk_hide(utk_widget *w)
{
	w->hide();
}

UTK_API int utk_is_visible(utk_widget *w)
{
	return w->is_visible();
}

UTK_API void utk_set_focus(utk_widget *w, int onoff)
{
	w->set_focus(onoff);
}

UTK_API int utk_get_focus(utk_widget *w)
{
	return w->get_focus() ? 1 : 0;
}

UTK_API void utk_add_child(utk_widget *w, utk_widget *c)
{
	w->add_child(c);
}

UTK_API utk_widget *utk_get_child(utk_widget *w)
{
	return w->get_child();
}

UTK_API utk_widget *utk_get_child_at(utk_widget *w, int x, int y)
{
	return w->get_child_at(x, y);
}

UTK_API int utk_remove_child(utk_widget *w, utk_widget *c)
{
	return w->remove_child(c) ? 0 : -1;
}

UTK_API void utk_set_parent(utk_widget *w, utk_widget *par)
{
	w->set_parent(par);
}

UTK_API utk_widget *utk_get_parent(utk_widget *w)
{
	return w->get_parent();
}

UTK_API utk_widget *utk_get_window(utk_widget *w)
{
	return w->get_window();
}

UTK_API void utk_rise(utk_widget *w)
{
	w->rise();
}

UTK_API void utk_sink(utk_widget *w)
{
	w->sink();
}

UTK_API utk_widget *utk_get_popup(utk_widget *w)
{
	return w->get_popup();
}

UTK_API void utk_set_popup(utk_widget *w, utk_widget *popup)
{
	assert(dynamic_cast<utk::PopupMenu*>(popup));
	w->set_popup((utk::PopupMenu*)popup);
}

UTK_API void utk_show_popup(utk_widget *w, int x, int y)
{
	w->show_popup(x, y);
}

UTK_API int utk_hit_test(utk_widget *w, int x, int y)
{
	return w->hit_test(x, y);
}

UTK_API void utk_link_bool(utk_widget *w, int *ptr)
{
	/* XXX dodgy */
	w->set_link((bool*)ptr);
}

UTK_API void utk_link_int(utk_widget *w, int *ptr)
{
	w->set_link(ptr);
}

UTK_API void utk_link_float(utk_widget *w, float *ptr)
{
	w->set_link(ptr);
}

UTK_API void utk_link_str(utk_widget *w, char *ptr, int bufsz)
{
	w->set_link(ptr, bufsz);
}

UTK_API void utk_set_callback(utk_widget *w, int evtype, utk_callback_func func, void *data)
{
	w->set_callback(evtype, func, data);
}

UTK_API utk_callback_func utk_get_callback(utk_widget *w, int evtype)
{
	return w->get_callback(evtype);
}

UTK_API void *utk_get_callback_data(utk_widget *w, int evtype)
{
	return w->get_callback_data(evtype);
}

UTK_API void utk_set_widget_data(utk_widget *w, void *data)
{
	w->set_user_data(data);
}

UTK_API void *utk_get_widget_data(utk_widget *w)
{
	return w->get_user_data();
}


/* window */
UTK_API utk_widget *utk_create_window(utk_widget *par, int x, int y, int w, int h, const char *title)
{
	return utk::create_window(par, x, y, w, h, title);
}

UTK_API void utk_destroy_window(utk_widget *w)
{
	utk::destroy_window(w);
}


}	// extern "C"
