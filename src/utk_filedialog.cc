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

#include <vector>
#include <string>
#include <list>
#include <algorithm>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#if defined(unix) || defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#elif defined(WIN32) || defined(__WIN32__)
#include <direct.h>
#include "w32_dirent.h"
#include <shlobj.h>
#else
#error "your OS is not supported currently, or OS detection failed"
#endif

#ifdef UBERTK_PCRE
#include <pcre.h>
#endif
#include "ubertk.h"
#include "utk_filedialog.h"
#include "utk_common.h"

namespace utk {

static bool is_dir(const char *name, const char *path);

FileDialogFilter::~FileDialogFilter() {}

FileDialogExtFilter::FileDialogExtFilter(const char *name, const char *exts)
{
	char buff[256];
	int blen = 0;
	this->name = strdup(name);

	if (exts) {
		for (int i=0;exts[i];i++) {
			if (exts[i] == ';') {
				buff[blen] = 0;
				blen = 0;
				add_ext(buff);
			} else {
				buff[blen++] = exts[i];
			}
		}
		if (blen) {
			buff[blen] = 0;
			add_ext(buff);
		}
	}
}

FileDialogExtFilter::~FileDialogExtFilter()
{
	std::list<char*>::iterator iter = extlist.begin();
	while (iter != extlist.end()) {
		free(*iter);
		iter++;
	}

	free(name);
}

void FileDialogExtFilter::add_ext(const char *ext)
{
	extlist.push_back(strdup(ext));
}

const char *FileDialogExtFilter::get_name() const
{
	return name;
}

bool FileDialogExtFilter::accept(const char *name) const
{
	if (extlist.size() == 0) return true;

	std::list<char*>::const_iterator iter = extlist.begin();
	for (; iter != extlist.end(); iter++) {
		char	*ext = *iter;
		size_t	len = strlen(ext), nlen = strlen(name);

		if (!len) return true;

		if (len > nlen) continue;

		if (!strcmp(name + (nlen - len), ext))
			return true;
	}

	return false;
}

FileDialogRegExpFilter::FileDialogRegExpFilter(const char *name, const char *regexp, bool wildcard)
{
#ifdef UBERTK_PCRE
	const char	*err;
	int			off;
#endif
	char		*code = strdup(regexp);
	char		buf[512];
	this->name = strdup(name);

	if (wildcard) {
	    const char *esc_chars = "()[]|+.^$";
	    const char *wild_chars = "*?";
	    char *rptr = buf, *spat = code;

		while (*spat && rptr < buf + (sizeof buf - 1)) {
	        if(strchr(esc_chars, *spat)) {
				*rptr++ = '\\';
			} else if(strchr(wild_chars, *spat)) {
	            *rptr++ = '.';
			}
			*rptr++ = *spat++;
		}
		*rptr = 0;

		free(code);
		code = buf;
	}

#ifdef UBERTK_PCRE
	rxprog = pcre_compile(code, 0, &err, &off, 0);
#endif

	if (!wildcard) free(code);
}

FileDialogRegExpFilter::~FileDialogRegExpFilter()
{
	free(name);
}

const char *FileDialogRegExpFilter::get_name() const
{
	return name;
}

bool FileDialogRegExpFilter::accept(const char *name) const
{
#ifdef UBERTK_PCRE
	return !rxprog || pcre_exec((pcre*)rxprog, 0, name, strlen(name), 0, 0, 0, 0) >= 0;
#else
	return true;
#endif
}

void FileDialog::extlist_modify_handler(Event *event, void *data)
{
	((FileDialog*)data)->set_filter();
}

void FileDialog::listb_modify_handler(Event *event, void *data)
{
	FileDialog	*dlg = (FileDialog*)data;
	dlg->filename->set_text(dlg->listb->get_selected_text());
}

void FileDialog::select_button_handler(Event *event, void *data)
{
	FileDialog *dlg = (FileDialog*)data;
	const char *fname = dlg->filename->get_text();
	char cwd[1024];
	getcwd(cwd, sizeof cwd);

	// empty filename? off...
	if (!fname[0]) return;

	// absolute path? load it
	if (fname[0] == '/' || (fname[0] && fname[1] == ':')) {
		if (!chdir(fname)) {
			chdir(cwd);
			dlg->set_path(fname);
		} else {
			message_dialog("Invalid absolute path", MSG_TYPE_ERROR);
		}
		return;
	}

	// a directory in the directory shown? load it
	std::string	full(dlg->get_path());
	if (full.size() && (full[full.size()-1] == '/' || full[full.size()-1] == '\\')) {
		full = full.substr(0, full.size() - 1);
	}
#ifdef WIN32
	full.append("\\");
#else
	full.append("/");
#endif
	full.append(fname);
	if (!chdir(full.c_str())) {
		chdir(cwd);
		dlg->set_path(full.c_str());
		return;
	}

	// a file in the directory shown? call EVENT_NOTIFY
	FILE *f = fopen(dlg->get_filename(), "wb");
	if(f) {
		fclose(f);
		Event ev;
		ev.widget = dlg;
		dlg->callback(&ev, EVENT_NOTIFY);
		return;
	}

	// regexp...

	dlg->custom_regexp();
}

void FileDialog::filename_notify_handler(Event *event, void *data)
{
	select_button_handler(event, data);
}

void FileDialog::listb_click_handler(Event *event, void *data)
{
	FileDialog	*dlg = (FileDialog*)data;
	ClickEvent	*cev = dynamic_cast<ClickEvent*>(event);

	if (!cev) return;
	if (cev->time - dlg->click_time < 400 && abs(cev->x - dlg->click_pos.x) < 4 && abs(cev->y - dlg->click_pos.y) < 4) {
		select_button_handler(event, data);
	}
	dlg->click_time = cev->time;
	dlg->click_pos = IVec2(cev->x, cev->y);
}

FileDialog::FileDialog(const char *title)
{
	visible = false;

	set_text(title);

	filter = 0;
	rxfilter = 0;

	full_filename = 0;

//	regexp = 0;
	show_hidden = false;
	path = 0;
	listb = 0;

	click_time = 0;
}

FileDialog::~FileDialog()
{
	std::list<FileDialogFilter*>::const_iterator iter = filters.begin();
	while (iter != filters.end()) {
		FileDialogFilter	*filter = *iter;
		delete filter;
		iter++;
	}

	if (rxfilter)
		delete rxfilter;

	if (full_filename)
		free(full_filename);

	free(path);
}

bool FileDialog::accept_file(const char *name) const
{
	if (filter) {
		return filter->accept(name);
	} else {
		return true;
	}
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
			//bool match = !regexp || pcre_exec((pcre*)regexp, 0, ent->d_name, strlen(ent->d_name), 0, 0, 0, 0) >= 0;
			bool match = accept_file(ent->d_name);

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

void FileDialog::set_filter()
{
	std::list<FileDialogFilter*>::iterator	iter = filters.begin();

	filter = 0;
	int i=0;

	for (; iter != filters.end(); iter++) {
		if (i == filterbox->get_selected()) filter = *iter;
		i++;
	}

	refresh();
}

void FileDialog::custom_regexp()
{
	if (rxfilter)
		delete rxfilter;
	filter = rxfilter = new FileDialogRegExpFilter("", filename->get_text(), !use_regexp->is_checked());
	refresh();
}

void FileDialog::set_path(const char *path)
{
	free(this->path);
	this->path = strdup(path);
	pathentry->set_text(path);
	refresh();
}

const char *FileDialog::get_path() const
{
	return path;
}

const char *FileDialog::get_filename()
{
	char	ffn[1024];
	char	*tmppath = strdup(path);

	char	dc = tmppath[strlen(tmppath) - 1];
	if (tmppath[0] && (dc == '/' || dc == '\\')) tmppath[strlen(tmppath) - 1] = 0;

#ifdef WIN32
	sprintf(ffn, "%s\\%s", tmppath, filename->get_text());
#else
	sprintf(ffn, "%s/%s", tmppath, filename->get_text());
#endif

	free(tmppath);
	if (full_filename)
		free(full_filename);
	full_filename = strdup(ffn);
	return full_filename;
}

void FileDialog::add_filter(FileDialogFilter *filter)
{
	bool wasempty = filters.empty();
	filters.push_back(filter);
	filterbox->add_item(filter->get_name());

	if(wasempty) {
		filterbox->select(0);
	}
}

void FileDialog::refresh()
{
	fill_filelist();
}


#if defined(WIN32) || defined(__WIN32__)
#define getcwd(a, b)	_getcwd(a, b)
#endif

// up
static void up_click_handler(Event *event, void *data)
{
	FileDialog	*fd = (FileDialog*)data;

	if (!fd->get_path()[0]) return;

	char		*updir = strdup(fd->get_path());
	char		*str = updir + strlen(updir) - 1;

	for (;str != updir;str--) {
		#ifdef WIN32
		if (*str == '\\' && *(str + 1)) {
		#else
		if (*str == '/' && *(str + 1)) {
		#endif
			if (str - 1 > updir && *(str - 1) != ':') {
				*str = 0;
			} else if (*(str - 1) == ':') {
				*(str + 1) = 0;
			}
			break;
		}
	}

	fd->set_path(updir);

	free(updir);
}

// home
static void home_click_handler(Event *event, void *data)
{
#if defined(WIN32) && !(defined(GNUC) || defined(__GNUC__))
	char	home[MAX_PATH];
	home[0] = 0;
	SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, home);
#else
	char	home[1024];
	strcpy(home, getenv("HOME"));
#endif
	((FileDialog*)data)->set_path(home);
}

// mkdir
static void mkdir_ok_handler(Event *event, void *data)
{
	FileDialog	*fd = (FileDialog*)data;
	std::string	newdir(fd->get_path());
	if (newdir.size() && (newdir[newdir.size()-1] == '/' || newdir[newdir.size()-1] == '\\')) {
		newdir = newdir.substr(0, newdir.size() - 1);
	}
#ifdef WIN32
	newdir.append("\\");
#else
	newdir.append("/");
#endif
	newdir.append(((Entry*)event->widget)->get_text());
	destroy_dialog(event->widget->get_window());

#ifdef WIN32
	int r = mkdir(newdir.c_str());
#else
	int r = mkdir(newdir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif

	if (r != 0) {
		std::string	error("Cannot create directory `");
		error.append(newdir);
		error.append("'");
		message_dialog(error.c_str(), MSG_TYPE_ERROR);
		return;
	}

	fd->set_path(newdir.c_str());
}

static void mkdir_click_handler(Event *event, void *data)
{
	input_dialog("New directory's name:", "Make directory", 0, mkdir_ok_handler, data);
}

// dialog
static void filedialog_cancel_handler(Event *event, void *data)
{
	Widget *w = event->widget;
	if(w) w = w->get_window();

	if(w) {
		destroy_dialog((Dialog*)w);
	}
}

FileDialog *file_dialog(unsigned int type, const char *fname, FileDialogFilter *filter,
		const char *start_dir, Callback func, void *cdata)
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
	dlg->set_callback(EVENT_NOTIFY, func, cdata);
	WinFrame *frm = new WinFrame(dlg);
	root->add_child(frm);
	dlg->rise();
	dlg->show();

	if(!start_dir) {
		getcwd(buf, sizeof buf);
		start_dir = buf;
	}
	dlg->path = (char*)malloc(strlen(start_dir) + 1);
	strcpy(dlg->path, start_dir);

	VBox *main_vbox = create_vbox(dlg);

	HBox *path_hbox = create_hbox(main_vbox);
	create_label(path_hbox, "path:");
	dlg->pathentry = create_entry(path_hbox, start_dir, 290);
	dlg->pathentry->set_readonly(true);

	HBox *list_hbox = create_hbox(main_vbox);

	VBox *bmark_vbox = create_vbox(list_hbox);
	create_button(bmark_vbox, "up", 65, 0, up_click_handler, dlg);
	create_button(bmark_vbox, "home", 65, 0, home_click_handler, dlg);
	create_button(bmark_vbox, "mkdir", 65, 0, mkdir_click_handler, dlg);
	create_checkbox(bmark_vbox, "hidden", false);
	dlg->use_regexp = create_checkbox(bmark_vbox, "regexp", false);

	dlg->listb = create_listbox(list_hbox, 258, 200);
	dlg->listb->set_spacing(0);
	dlg->listb->set_callback(EVENT_MODIFY, FileDialog::listb_modify_handler, dlg);
	dlg->listb->set_callback(EVENT_CLICK, FileDialog::listb_click_handler, dlg);

	if(dlg->fill_filelist() == false) {
		destroy_dialog(dlg);
		return 0;
	}


	HBox *fname_hbox = create_hbox(main_vbox);
	create_label(fname_hbox, "filename:");
	dlg->filename = create_entry(fname_hbox, "", 250);
	dlg->filename->set_callback(EVENT_NOTIFY, FileDialog::filename_notify_handler, dlg);

	HBox *filter_hbox = create_hbox(main_vbox);
	create_label(filter_hbox, "filter:");
	dlg->filterbox = create_combobox(filter_hbox);
	dlg->filterbox->set_size(280, dlg->filterbox->get_height());
	dlg->filterbox->layout();
	dlg->filterbox->set_callback(EVENT_MODIFY, FileDialog::extlist_modify_handler, dlg);
	dlg->filterbox->set_readonly(false);

	create_spacer(main_vbox, 10);

	HBox *bn_hbox = create_hbox(main_vbox);
	create_spacer(bn_hbox, 50);
	create_button(bn_hbox, type == FILE_DIALOG_OPEN ? "open" : "save", FileDialog::select_button_handler, dlg);
	create_spacer(bn_hbox, 8);
	create_button(bn_hbox, "cancel", filedialog_cancel_handler);

	if (filter) {
		dlg->add_filter(filter);
	}

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

}
