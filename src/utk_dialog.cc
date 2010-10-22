/*
ubertk is a flexible GUI toolkit targetted towards graphics applications.
Copyright (C) 2007 - 2008 John Tsiombikas <nuclear@member.fsf.org>,
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
#include "utk_config.h"
#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>
#include <string.h>
#include <errno.h>

#if defined(unix) || defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#elif defined(WIN32) || defined(__WIN32__)
#include <direct.h>
#include "w32_dirent.h"
#else
#error "your OS is not supported currently, or OS detection failed"
#endif


#ifdef UBERTK_PCRE
#include <pcre.h>
#endif

#include "ubertk.h"
#include "utk_dialog.h"
#include "utk_common.h"

namespace utk {

Dialog::Dialog()
{
	modal = true;
}
Dialog::~Dialog() {}



Dialog *create_dialog(Widget *parent, int x, int y, int w, int h, const char *title)
{
	Dialog *dlg = new Dialog;
	dlg->set_pos(x, y);
	dlg->set_size(w, h);
	dlg->set_text(title);

	WinFrame *frm = new WinFrame(dlg);
	parent->add_child(frm);

	dlg->rise();
	return dlg;
}

void destroy_dialog(Widget *w)
{
	destroy_window(w);
}


static void def_dialog_handler(Event *event, void *data)
{
	Widget *w = event->widget;
	if(w) w = w->get_window();

	if(w) {
		destroy_window(w);
	}
}

Dialog *message_dialog(const char *msg, unsigned int type, unsigned int bn_mask, Callback func, void *cdata)
{
	static const char *type_str[] = {
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

	Dialog *dlg = create_dialog(root, x, y, width, height, type_str[type]);

	dlg->add_child(vbox);
	dlg->show();

	return dlg;
}

Dialog *message_dialog(const char *msg, unsigned int type, Callback func, void *cdata)
{
	unsigned int bn_mask = (type == MSG_TYPE_QUESTION ? MSG_BN_YES_NO : MSG_BN_OK);
	return message_dialog(msg, type, bn_mask, func, cdata);
}


// -------------------- file dialogs --------------------
static bool is_dir(const char *name, const char *path);
static void entry_modify_handler(Event *event, void *data);
static void bn_handler(Event *event, void *data);

FileDialog::FileDialog()
{
	regexp = 0;
	show_hidden = false;
	path = 0;
	listb = 0;
}

FileDialog::~FileDialog()
{
	delete [] path;
}

bool FileDialog::fill_filelist()
{
	std::vector<std::string> file_list;
	std::vector<std::string> dir_list;

	DIR *dir;
	if(!(dir = opendir(path))) {
		utk_error("opendir failed on %s: %s\n", path, strerror(errno));
		return false;
	}

	struct dirent *ent;
	while((ent = readdir(dir))) {
		if(is_dir(ent->d_name, path)) {
			if((show_hidden || ent->d_name[0] != '.')) {
				dir_list.push_back(ent->d_name);
			}
		} else {
#ifdef UBERTK_PCRE
			bool match = !regexp || pcre_exec((pcre*)regexp, 0, ent->d_name, strlen(ent->d_name), 0, 0, 0, 0) >= 0;
#else
			bool match = true;
#endif
			if((show_hidden || ent->d_name[0] != '.') && match) {
				file_list.push_back(ent->d_name);
			}
		}
	}
	closedir(dir);

	sort(file_list.begin(), file_list.end());
	sort(dir_list.begin(), dir_list.end());

	listb->clear();

	for(size_t i=0; i<dir_list.size(); i++) {
		listb->add_item((dir_list[i] + "/").c_str());
	}

	for(size_t i=0; i<file_list.size(); i++) {
		listb->add_item(file_list[i].c_str());
	}

	return true;
}


#if defined(WIN32) || defined(__WIN32__)
#define getcwd(a, b)	_getcwd(a, b)
#endif

static void filedialog_cancel_handler(Event *event, void *data)
{
	Widget *w = event->widget;
	if(w) w = w->get_window();

	if(w) {
		destroy_dialog((Dialog*)w);
	}
}

FileDialog *file_dialog(unsigned int type, const char *fname, const char *filter, const char *start_dir, Callback func, void *cdata)
{
	static char buf[1024];
	Widget *root = get_root_widget();
	int width = 350;
	int height = 350;
	int x = (root->get_size().x - width) / 2;
	int y = (root->get_size().y - height) / 2;

	FileDialog *dlg = new FileDialog;
	dlg->set_pos(x, y);
	dlg->set_size(width, height);
	dlg->set_text(type == FILE_DIALOG_OPEN ? "open file" : "save file");
	WinFrame *frm = new WinFrame(dlg);
	root->add_child(frm);
	dlg->rise();
	dlg->show();

	if(!start_dir) {
		getcwd(buf, sizeof buf);
		start_dir = buf;
	}
	dlg->path = new char[strlen(start_dir) + 1];
	strcpy(dlg->path, start_dir);

	VBox *main_vbox = create_vbox(dlg);

	HBox *path_hbox = create_hbox(main_vbox);
	create_label(path_hbox, "path:");
	create_entry(path_hbox, start_dir, 290);	// TODO: also pass func to change the dir on modify

	HBox *list_hbox = create_hbox(main_vbox);

	VBox *bmark_vbox = create_vbox(list_hbox);
	create_button(bmark_vbox, "up", 65);
	create_button(bmark_vbox, "home", 65);
	create_button(bmark_vbox, "mkdir", 65);
	create_checkbox(bmark_vbox, "hidden", false);

	dlg->listb = create_listbox(list_hbox, 258, 200);
	dlg->listb->set_spacing(0);

#ifdef UBERTK_PCRE
	if(filter && *filter) {
		const char *err_str;
		int offs;
		dlg->regexp = pcre_compile(filter, 0, &err_str, &offs, 0);
	}
#endif

	if(dlg->fill_filelist() == false) {
		destroy_dialog(dlg);
		return 0;
	}


	HBox *fname_hbox = create_hbox(main_vbox);
	create_label(fname_hbox, "filename:");
	create_entry(fname_hbox, "", 250);

	HBox *filter_hbox = create_hbox(main_vbox);
	create_label(filter_hbox, "filter:");
	create_entry(filter_hbox, "", 200, entry_modify_handler)->set_name("fdlg_filter");
	create_spacer(filter_hbox, 5);
	create_checkbox(filter_hbox, "regexp", false);

	create_spacer(main_vbox, 10);

	HBox *bn_hbox = create_hbox(main_vbox);
	create_spacer(bn_hbox, 50);
	create_button(bn_hbox, type == FILE_DIALOG_OPEN ? "open" : "save");
	create_spacer(bn_hbox, 8);
	create_button(bn_hbox, "cancel", filedialog_cancel_handler);

	return dlg;
}

static bool is_dir(const char *name, const char *path)
{
	char *full_name;

	if(!(full_name = (char*)malloc(strlen(name) + strlen(path) + 2))) {
		utk_error("malloc failed");
		return false;
	}
	sprintf(full_name, "%s/%s", path, name);

#if defined(unix) || defined(__unix__)
	struct stat st;
	stat(full_name, &st);
	free(full_name);

	return S_ISDIR(st.st_mode);
#else
	DIR *dir = opendir(full_name);
	free(full_name);
	if(dir) {
		closedir(dir);
		return true;
	}
#endif
	return false;
}


static void entry_modify_handler(Event *event, void *data)
{
	if(strcmp(event->widget->get_name(), "fdlg_filter") == 0) {
	}
}

static void bn_handler(Event *event, void *data)
{

}

} // end of utk namespace
