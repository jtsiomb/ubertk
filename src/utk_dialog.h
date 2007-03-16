#ifndef UTK_DIALOG_H_
#define UTK_DIALOG_H_

#include <utk_widget.h>

namespace utk {

class Dialog : public Window {
public:
	Dialog();
	virtual ~Dialog();
};

class FileDialog : public Dialog {
	void *regexp;	// opaque, because I don't want to expose the regexp lib in the header
	bool show_hidden;

	const char *path;
	ListBox *listb;

	bool fill_filelist();

public:
	FileDialog();
	virtual ~FileDialog();

	friend FileDialog *file_dialog(unsigned int type, const char *fname, const char *filter, const char *start_dir, Callback func, void *cdata);
};

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

Dialog *create_dialog(Widget *parent, int x, int y, int w, int h, const char *title);
void destroy_dialog(Widget *w);

Dialog *message_dialog(const char *msg, unsigned int type, unsigned int bn_mask, Callback func = 0, void *cdata = 0);
Dialog *message_dialog(const char *msg, unsigned int type, Callback func = 0, void *cdata = 0);

FileDialog *file_dialog(unsigned int type, const char *fname, const char *filter, const char *start_dir, Callback func = 0, void *cdata = 0);

} // end utk namespace

#endif	// UTK_DIALOG_H_
