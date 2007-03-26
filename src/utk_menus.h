#ifndef UBERTK_MENUS_H_
#define UBERTK_MENUS_H_

#include <utk_widget.h>
#include <utk_drawable.h>
#include <utk_container.h>
#include <utk_popup.h>

namespace utk {

class PopupMenuItem : public Drawable
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

class PopupMenuSeparator : public PopupMenuItem
{
	public:
	PopupMenuSeparator();
	
	virtual void draw() const;
};

class PopupMenu : public Popup
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


PopupMenu *create_popup_menu(bool show=false);

}

#endif
