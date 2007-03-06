#ifndef UTK_DIALOG_H_
#define UTK_DIALOG_H_

#include <utk_widget.h>

namespace utk {

enum {
	MSG_BN_OK			= 1, // 0001
	MSG_BN_CANCEL		= 2, // 0010
	MSG_BN_OK_CANCEL	= 3, // 0011
	MSG_BN_YES			= 4, // 0100
	MSG_BN_NO			= 8, // 1000
	MSG_BN_YES_NO		= 12 // 1100
};

enum {
	MSG_TYPE_QUESTION,
	MSG_TYPE_INFO,
	MSG_TYPE_WARNING,
	MSG_TYPE_ERROR
};

enum {
	FILE_DIALOG_OPEN,
	FILE_DIALOG_SAVE
};

enum { DLG_MODAL = (1 << 15) };

Widget *message_dialog(const char *msg, unsigned int type, unsigned int bn_mask, Callback func = 0, void *cdata = 0);
Widget *message_dialog(const char *msg, unsigned int type, Callback func = 0, void *cdata = 0);

Widget *file_dialog(unsigned int type, const char *fname, const char *filter, const char *start_dir, Callback func = 0, void *cdata = 0);

void destory_dialog(Widget *w);

} // end utk namespace

#endif	// UTK_DIALOG_H_
