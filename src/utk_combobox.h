/*
ubertk is a flexible GUI toolkit targetted towards graphics applications.
Copyright (C) 2007 - 2018 John Tsiombikas <nuclear@member.fsf.org>,
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

#ifndef UTK_COMBOBOX_H_
#define UTK_COMBOBOX_H_

#include <utk_config.h>
#include <utk_events.h>
#include <utk_button.h>
#include <utk_entry.h>
#include <utk_popup.h>
#include <utk_listbox.h>
#include <utk_container.h>

namespace utk {

class UTK_API ComboBox : public utk::HBox
{
protected:
	Entry	*entry;
	Button	*reveal;
	Popup	*list_popup;
	ListBox	*list;

public:
	ComboBox();
	virtual ~ComboBox();

	virtual void set_size(int w, int h);

	virtual void show_list();

	virtual void add_item(const char *text);
	virtual void add_item(utk::Widget *widget);
	virtual void remove_item(int pos = -1);

	virtual void select(int sel);
	virtual int get_selected() const;
	virtual const char *get_selected_text() const;

	virtual void text_to_selected_item();

	virtual void set_readonly(bool readonly);
	virtual bool is_readonly() const;
};

UTK_API ComboBox *create_combobox(utk::Widget *parent);

}

#endif
