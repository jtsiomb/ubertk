#include "ubertk.h"
#include "utk_dialog.h"
#include "utk_common.h"

namespace utk {

Widget *message_dialog(const char *msg, unsigned int type, unsigned int bn_mask, Callback func)
{
	static char *type_str[] = {
		"Question",
		"Info",
		"Warning",
		"Error"
	};
	int lines = 0;
	int max_len = 0;

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

	int width = MAX(max_len + 10, 100);
	int height = lines * gfx::text_spacing() + 20;
	int x = (root->get_size().x - width) / 2;
	int y = (root->get_size().y - height) / 2;
	
	Window *win = create_window(root, x, y, width, height, type_str[type]);
	win->add_child(vbox);
	win->show();

	return win;
}

Widget *message_dialog(const char *msg, unsigned int type, Callback func)
{
	unsigned int bn_mask = (type == MSG_TYPE_QUESTION ? MSG_BN_YES_NO : MSG_BN_OK);
	return message_dialog(msg, type, bn_mask, func);
}

} // end of utk namespace
