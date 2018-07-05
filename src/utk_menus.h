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

#ifndef UTK_MENUS_H_
#define UTK_MENUS_H_

#include <utk_config.h>
#include <utk_widget.h>
#include <utk_drawable.h>
#include <utk_container.h>
#include <utk_popup.h>

namespace utk {

class UTK_API PopupMenuItem : public Drawable
{
	friend class PopupMenu;

	protected:
	utk::Widget		*widget;
	class PopupMenu *master;
	class PopupMenu *submenu;

	public:
	PopupMenuItem(utk::Widget *widget);
	virtual ~PopupMenuItem();

	virtual utk::Widget *get_child_at(int x, int y);
	virtual void set_size(int w, int h);

	virtual Widget *handle_event(utk::Event *event);

	virtual void draw() const;

	virtual PopupMenuItem *add_item(PopupMenuItem *item, utk::Callback cb=0);
	virtual PopupMenuItem *add_item(const char *item, utk::Callback cb=0);
	virtual void add_separator();
};

class UTK_API PopupMenuSeparator : public PopupMenuItem
{
	public:
	PopupMenuSeparator();

	virtual void draw() const;
};

class UTK_API PopupMenu : public Popup
{
	friend class PopupMenuItem;

	protected:
	VBox			*vbox;
	PopupMenu		*master;
	PopupMenu		*vis_sub;	// visible submenu
	unsigned int	showtime;

	virtual void update_size();

	public:
	bool		delete_master;	// if true, then once the master popup is cancelled
								// everything gets deleted.

	PopupMenu();
	virtual ~PopupMenu();

	virtual void set_size(int w, int h);

	virtual Widget *handle_event(utk::Event *event);

	virtual PopupMenuItem *add_item(PopupMenuItem *item, utk::Callback cb=0);
	virtual PopupMenuItem *add_item(const char *item, utk::Callback cb=0);
	virtual void add_separator();

	virtual void run(int x=-1, int y=-1);
	virtual void cancel();
};


UTK_API PopupMenu *create_popup_menu(bool show=false);

}

#endif	// UTK_API
