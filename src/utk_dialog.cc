#include "ubertk.h"
#include "utk_dialog.h"
#include "utk_common.h"

namespace utk {

static void def_dialog_handler(Event *event, void *data)
{
	Widget *w = event->widget;
	while(w && !dynamic_cast<Window*>(w)) {
		w = w->get_parent();
	}

	if(w) {
		destroy_window(w);
	}
}

Widget *message_dialog(const char *msg, unsigned int type, unsigned int bn_mask, Callback func, void *cdata)
{
	static char *type_str[] = {
		"Question",
		"Info",
		"Warning",
		"Error"
	};
	int lines = 0;
	int max_len = 0;

	if(!func) func = def_dialog_handler;

	Widget *root = get_root_widget();
	VBox *vbox = new VBox;

	char *buf = (char*)malloc(strlen(msg) + 1);
	if(!buf) {
		utk_error("malloc failed");
		delete vbox;
		return 0;
	}

	while(*msg) {
		const char *sptr = msg;
		char *dptr = buf;

		while(*sptr && *sptr != '\n') {
			*dptr++ = *sptr++;
		}
		*dptr = 0;

		msg = *sptr ? sptr + 1 : sptr;

		create_label(vbox, buf);
		
		int len = gfx::text_width(buf, 18);
		if(len > max_len) {
			max_len = len;
		}
		lines++;
	}

	free(buf);

	int bn_width = 0;
	if(bn_mask) {
		HBox *hbox = create_hbox(vbox);
	
		if(bn_mask & MSG_BN_OK) {
			create_button(hbox, "OK", func);
		}
		if(bn_mask & MSG_BN_CANCEL) {
			create_button(hbox, "Cancel", func);
		}
		if(bn_mask & MSG_BN_YES) {
			create_button(hbox, "Yes", func);
		}
		if(bn_mask & MSG_BN_NO) {
			create_button(hbox, "No", func);
		}

		bn_width = hbox->get_width();
	}


	int width = MAX(max_len, bn_width) + 10;
	int text_spacing = gfx::text_spacing();
	int height = lines * text_spacing + 10 + (bn_mask ? text_spacing + 10 : 0);
	int x = (root->get_size().x - width) / 2;
	int y = (root->get_size().y - height) / 2;
	
	Window *win = create_window(root, x, y, width, height, type_str[type]);
	win->add_child(vbox);
	win->show();

	return win;
}

Widget *message_dialog(const char *msg, unsigned int type, Callback func, void *cdata)
{
	unsigned int bn_mask = (type == MSG_TYPE_QUESTION ? MSG_BN_YES_NO : MSG_BN_OK);
	return message_dialog(msg, type, bn_mask, func, cdata);
}

Widget *file_dialog(unsigned int type, const char *fname, const char *filter, const char *start_dir, Callback func, void *cdata)
{
	char *cwd_pname = "/tmp";
	Widget *root = get_root_widget();
	int width = 350;
	int height = 400;
	int x = (root->get_size().x - width) / 2;
	int y = (root->get_size().y - height) / 2;

	Window *win = create_window(root, x, y, width, height, type == FILE_DIALOG_OPEN ? "open file" : "save file");
	win->show();
	VBox *main_vbox = create_vbox(win);

	HBox *path_hbox = create_hbox(main_vbox);
	create_label(path_hbox, "path:");
	Entry *en_path = create_entry(path_hbox, cwd_pname, 150);	// TODO: also pass func to change the dir on modify

	HBox *list_hbox = create_hbox(main_vbox);

	VBox *bmark_vbox = create_vbox(list_hbox);
	create_button(bmark_vbox, "go home");
	create_button(bmark_vbox, "new dir");
	create_button(bmark_vbox, "rename");

	ListBox *listb = create_listbox(list_hbox, 200, 200);
	// TODO: populate and add callbacks
	

	HBox *fname_hbox = create_hbox(main_vbox);
	create_label(fname_hbox, "filename:");
	create_entry(fname_hbox, "", 100);

	HBox *filter_hbox = create_hbox(main_vbox);
	create_label(filter_hbox, "filter:");
	create_entry(filter_hbox, "", 100);

	HBox *bn_hbox = create_hbox(main_vbox);
	create_button(bn_hbox, type == FILE_DIALOG_OPEN ? "open" : "save");
	create_button(bn_hbox, "cancel");

	return win;
}

void destory_dialog(Widget *w)
{
	destroy_window(w);
}

} // end of utk namespace
