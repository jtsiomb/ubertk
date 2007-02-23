#ifndef UTK_DIALOG_H_
#define UTK_DIALOG_H_

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

enum { DLG_MODAL = (1 << 15) };

Window *create_message_dialog(const char *msg, unsigned int type, unsigned int bn_mask = 0);

} // end utk namespace

#endif	// UTK_DIALOG_H_
