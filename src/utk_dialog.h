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

#ifndef UTK_DIALOG_H_
#define UTK_DIALOG_H_

#include <utk_widget.h>

namespace utk {

class Dialog : public Window {
public:
	Dialog();
	virtual ~Dialog();
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

Dialog *input_dialog(const char *msg, const char *title = "Input", const char *deftext=0, Callback func = 0, void *cdata = 0);

} // end utk namespace

#endif	// UTK_DIALOG_H_
