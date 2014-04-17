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

#ifndef UTK_FILEDIALOG_H_
#define UTK_FILEDIALOG_H_

#include <utk_combobox.h>
#include <utk_dialog.h>

namespace utk {

class FileDialogFilter
{
public:
	virtual ~FileDialogFilter();

	virtual const char *get_name() const=0;
	virtual bool accept(const char *name) const=0;
};

class FileDialogExtFilter : public FileDialogFilter
{
protected:
	std::list<char*> extlist;
	char *name;

public:
	FileDialogExtFilter(const char *name, const char *exts);
	virtual ~FileDialogExtFilter();

	virtual void add_ext(const char *ext);

	virtual const char *get_name() const;
	virtual bool accept(const char *name) const;
};

class FileDialogRegExpFilter : public FileDialogFilter
{
protected:
	void *rxprog;
	char *regexp;
	char *name;
public:
	FileDialogRegExpFilter(const char *name, const char *regexp, bool wildcard);
	virtual ~FileDialogRegExpFilter();

	virtual const char *get_name() const;
	virtual bool accept(const char *name) const;
};

class FileDialog : public Dialog {
	std::list<FileDialogFilter*> filters;
	FileDialogFilter *filter;
	FileDialogRegExpFilter *rxfilter;
	bool show_hidden;

	char *path;
	Entry *pathentry;
	Entry *filename;
	ListBox *listb;
	ComboBox *filterbox;
	CheckBox *use_regexp;
	char *full_filename;
	int click_time;
	IVec2 click_pos;

	bool accept_file(const char *name) const;
	bool fill_filelist();
	void set_filter();
	void custom_regexp();

	static void select_button_handler(Event *event, void *data);
	static void extlist_modify_handler(Event *event, void *data);
	static void filename_notify_handler(Event *event, void *data);
	static void listb_modify_handler(Event *event, void *data);
	static void listb_click_handler(Event *event, void *data);

public:
	FileDialog(const char *title="Select file");
	virtual ~FileDialog();

	virtual void set_path(const char *path);
	virtual const char *get_path() const;
	virtual const char *get_filename();

	virtual void add_filter(FileDialogFilter *filter);

	virtual void refresh();

	friend FileDialog *file_dialog(unsigned int type, const char *fname, FileDialogFilter *filter, const char *start_dir, Callback func, void *cdata);
};

FileDialog *file_dialog(unsigned int type, const char *fname, FileDialogFilter *filter, const char *start_dir, Callback func = 0, void *cdata = 0);

}

#endif
